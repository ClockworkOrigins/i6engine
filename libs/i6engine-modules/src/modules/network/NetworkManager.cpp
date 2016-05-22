/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/modules/network/NetworkManager.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Logger.h"

#ifdef ISIXE_WITH_PROFILING
	#include "i6engine/utils/Profiling.h"
#endif /* ISIXE_WITH_PROFILING */

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/NetworkConfig.h"
#include "i6engine/api/facades/NetworkFacade.h"

#include "i6engine/modules/network/NetworkMailbox.h"

#include "boost/thread.hpp"

#include "m2etis/pubsub/PubSubSystem.h"

namespace i6e {
namespace modules {

	NetworkManager::NetworkManager(NetworkMailbox * mailbox) : _mailbox(mailbox), _sendPing(false), _counterSent(0), _counterReceived(), _ne(this), _pubsub(nullptr), _lastTickTime(api::EngineController::GetSingleton().getCurrentTime()), _connectionFailed(false), _usedChannels(), _pubSubLock() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		api::EngineController::GetSingletonPtr()->getNetworkFacade()->registerConnectCallback(boost::bind(&NetworkManager::connect, this, _1, _2));
		api::EngineController::GetSingletonPtr()->getNetworkFacade()->registerDisconnectCallback(boost::bind(&NetworkManager::disconnect, this));
		api::EngineController::GetSingletonPtr()->getNetworkFacade()->registerSubscribeCallback(boost::bind(&NetworkManager::subscribe, this, _1));
		api::EngineController::GetSingletonPtr()->getNetworkFacade()->registerUnsubscribeCallback(boost::bind(&NetworkManager::unsubscribe, this, _1));
		api::EngineController::GetSingletonPtr()->getNetworkFacade()->registerPublishCallback(boost::bind(&NetworkManager::publish, this, _1, _2));
	}

	NetworkManager::~NetworkManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_pubsub != nullptr) {
			disconnect();
		}
	}

	void NetworkManager::subscribe(uint16_t channel) {
		boost::mutex::scoped_lock sl(_pubSubLock);
		if (_pubsub == nullptr) {
			ISIXE_THROW_FAILURE("NetworkManager", "Tried to subscribe without having connected before");
		}
		if (_usedChannels.find(channel) == _usedChannels.end()) {
			_pubsub->subscribe<api::GameMessage>(m2etis::pubsub::ChannelName(channel), *this);
			_usedChannels[channel] = 0;
		}
		_usedChannels[channel]++;
	}

	void NetworkManager::unsubscribe(uint16_t channel) {
		boost::mutex::scoped_lock sl(_pubSubLock);
		if (_pubsub == nullptr) {
			ISIXE_THROW_FAILURE("NetworkManager", "Tried to unsubscribe without having connected before");
		}
		if (_usedChannels.find(channel) != _usedChannels.end()) {
			_usedChannels[channel]--;

			if (_usedChannels[channel] == 0) {
				_pubsub->unsubscribe<api::GameMessage>(m2etis::pubsub::ChannelName(channel));
				_usedChannels.erase(channel);
			}
		}
	}

	void NetworkManager::publish(uint16_t channel, const api::GameMessage::Ptr & msg) {
		boost::mutex::scoped_lock sl(_pubSubLock);
		if (_pubsub == nullptr) {
			ISIXE_LOG_WARN("NetworkManager", "Tried to publish without having connected before");
			return;
		}
		msg->getContent()->_sender = api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP();
#ifdef ISIXE_WITH_PROFILING
		msg->insertTimestamp("NetworkManager -> m2etis");
		if (utils::profiling::timeStamp) {
			ISIXE_LOG_INFO("Network", "Send:" << msg->getMessageInfo() << " " << msg->getTimestamps());
		}
#endif /* ISIXE_WITH_PROFILING */

		publishInternal(m2etis::pubsub::ChannelName(channel), _pubsub->createMessage<api::GameMessage>(m2etis::pubsub::ChannelName(channel), *msg));
	}

	bool NetworkManager::connect(const core::IPKey & remote, const core::IPKey & local) {
		boost::mutex::scoped_lock sl(_pubSubLock);
		if (_pubsub != nullptr) {
			return false;
		}
		_pubsub = new m2etis::pubsub::PubSubSystem(local.getIP(), local.getPort(), remote.getIP(), remote.getPort(), { remote.getIP() });

		_pubsub->registerExceptionCallback(m2etis::pubsub::CONNECTION_FAILED, boost::bind(&NetworkErrors::connectionFailed, &_ne, _1));
		_pubsub->registerExceptionCallback(m2etis::pubsub::CONNECTION_CLOSED, boost::bind(&NetworkErrors::connectionClosed, &_ne, _1));
		_pubsub->registerExceptionCallback(m2etis::pubsub::BINDFAILURE, boost::bind(&NetworkErrors::bindFailure, &_ne, _1));

		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

		if (_connectionFailed) {
			_connectionFailed = false;
			delete _pubsub;
			_pubsub = nullptr;
			return false;
		}

		ISIXE_LOG_INFO("NetworkManager", "connected");

		return true;
	}

	void NetworkManager::disconnect() {
		while (!_usedChannels.empty()) {
			boost::mutex::scoped_lock sl(_pubSubLock);
			_pubsub->unsubscribe<api::GameMessage>(m2etis::pubsub::ChannelName(_usedChannels.begin()->first));
			_usedChannels.erase(_usedChannels.begin());
		}

		boost::this_thread::sleep(boost::posix_time::milliseconds(100));

		boost::mutex::scoped_lock sl(_pubSubLock);
		delete _pubsub;
		_pubsub = nullptr;
	}

	// comes from m2etis
	void NetworkManager::deliverCallback(const m2etis::message::M2Message<api::GameMessage>::Ptr msg) {
		api::GameMessage::Ptr m = boost::make_shared<api::GameMessage>(*msg->payload);

#ifdef ISIXE_WITH_PROFILING
		m->insertTimestamp("m2etis -> NetworkManager");

		_counterReceived++;
#endif /* ISIXE_WITH_PROFILING */

		// only forward message to game if this node isn't the sender
		core::IPKey p = m->getContent()->_sender;
		if (api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP() == p) {
#ifdef ISIXE_WITH_PROFILING
			if (utils::profiling::timeStamp) {
				ISIXE_LOG_INFO("Network", "Receive:" << m->getMessageInfo() << " " << m->getTimestamps());
			}
#endif /* ISIXE_WITH_PROFILING */
			return;
		}

		_mailbox->NetworkNews(m);
	}

	void NetworkManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		// writes the amount of messages being sent during the last tick
		// As the counter isn't thread safe, the counter won't be
#ifdef ISIXE_WITH_PROFILING
		uint32_t a = _counterReceived;
		_counterReceived = 0;
		if (a != 0) {
			if (utils::profiling::numberMessages) {
				ISIXE_LOG_INFO("Messages received", a);
			}
		}
		a = _counterSent;
		_counterSent = 0;
		if (a != 0) {
			if (utils::profiling::numberMessages) {
				ISIXE_LOG_INFO("Messages sent", a);
			}
		}
#endif /* ISIXE_WITH_PROFILING */

		// Get current time.
		uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
		// Compute new game time (the difference between the starting time and the current time).
		uint64_t l = cT - _lastTickTime;
		if (l > 1000000) {
			if (_sendPing && api::EngineController::GetSingletonPtr()->getType() == api::GameType::SERVER) {
				api::GameMessage::Ptr msg(new api::GameMessage(api::messages::NetworkMessageType, api::network::NetPingRequest, core::Method::Update, new api::network::Network_Ping_Update(core::IPKey(), cT), core::Subsystem::Network));
				publish(PINGREQUEST_CHANNEL, msg);
			}
			_lastTickTime = cT;
		}
	}

	void NetworkManager::publishInternal(m2etis::pubsub::ChannelName channel, const m2etis::message::M2Message<api::GameMessage>::Ptr & msg) const {
#ifdef ISIXE_WITH_PROFILING
		const_cast<NetworkManager *>(this)->_counterSent++;
#endif /* ISIXE_WITH_PROFILING */

		try {
			_pubsub->publish<api::GameMessage>(channel, msg);
		} catch (std::runtime_error &) { // boost::system::system_error
			if (api::EngineController::GetSingletonPtr()->getType() == api::GameType::SERVER) {
				ISIXE_LOG_ERROR("NetworkManager", "Server isn't able to send message");
			} else {
				ISIXE_LOG_ERROR("NetworkManager", "Client isn't able to send message");

				// TODO: Maybe reconnect??? or ask GameDeveloper using a callback/interface-function
			}
		}
	}

	void NetworkManager::enablePing(bool enabled) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_sendPing = enabled;
	}

} /* namespace modules */
} /* namespace i6e */
