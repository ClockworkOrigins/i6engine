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

#include "i6engine/api/facades/NetworkFacade.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include <ifaddrs.h>
#endif

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/NetworkConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "clockUtils/sockets/TcpSocket.h"

namespace i6e {
namespace api {

	NetworkFacade::NetworkFacade() : _ownIP(), _subscribeInit(false), _unsubscribeInit(false), _publishInit(false), _subscribe(), _unsubscribe(), _publish(), _connectInit(), _disconnectInit(), _connect(), _disconnect(), _connected(false) { // TODO: (Daniel) refactor, bad code and only one 'slot' checked
		_IP = clockUtils::sockets::TcpSocket().getLocalIP();
	}

	NetworkFacade::~NetworkFacade() {
	}

	bool NetworkFacade::connect(const core::IPKey remoteIP, const uint16_t localPort) {
		_ownIP = core::IPKey(_IP, localPort);
		if (_connectInit) {
			if (_connect(remoteIP, _ownIP)) {
				_connected = true;

				return true;
			}
		}
		return false;
	}

	bool NetworkFacade::listen(const uint16_t localPort) {
		_ownIP = core::IPKey(_IP, localPort);
		if (_connectInit) {
			if (_connect(_ownIP, _ownIP)) {
				_connected = true;

				return true;
			}
		}
		return false;
	}

	void NetworkFacade::disconnect() {
		if (_disconnectInit) {
			_disconnect();

			_connected = false;
		}
	}

	void NetworkFacade::enablePing(bool enabled) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::NetworkMessageType, network::NetPingEnable, core::Method::Update, new network::Network_PingEnable_Update(enabled), core::Subsystem::Unknown);
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void NetworkFacade::subscribe(uint16_t channel) {
		if (_subscribeInit && _connected) {
			_subscribe(channel);
		}
	}

	void NetworkFacade::unsubscribe(uint16_t channel) {
		if (_unsubscribeInit && _connected) {
			_unsubscribe(channel);
		}
	}

	void NetworkFacade::publish(uint16_t channel, const GameMessage::Ptr & msg) {
		if (_publishInit && _connected) {
			_publish(channel, msg);
		}
	}

	void NetworkFacade::registerSubscribeCallback(const boost::function<void(uint16_t)> & f) {
		_subscribe = f;
		_subscribeInit = true;
	}

	void NetworkFacade::registerUnsubscribeCallback(const boost::function<void(uint16_t)> & f) {
		_unsubscribe = f;
		_unsubscribeInit = true;
	}

	void NetworkFacade::registerPublishCallback(const boost::function<void(uint16_t, const GameMessage::Ptr &)> & f) {
		_publish = f;
		_publishInit = true;
	}

	void NetworkFacade::registerConnectCallback(const boost::function<bool(const core::IPKey &, const core::IPKey &)> & f) {
		_connect = f;
		_connectInit = true;
	}

	void NetworkFacade::registerDisconnectCallback(const boost::function<void(void)> & f) {
		_disconnect = f;
		_disconnectInit = true;
	}

	void NetworkFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::NetworkMessageType, network::NetReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace modules */
} /* namespace i6e */
