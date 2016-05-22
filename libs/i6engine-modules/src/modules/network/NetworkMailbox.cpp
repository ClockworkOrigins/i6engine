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

#include "i6engine/modules/network/NetworkMailbox.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/NetworkConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"

#include "i6engine/modules/network/NetworkManager.h"

namespace i6e {
namespace modules {

	NetworkMailbox::NetworkMailbox() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	NetworkMailbox::~NetworkMailbox() {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	// from local MessagingSystem
	void NetworkMailbox::News(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getMessageType() == api::messages::NetworkMessageType) {
			if (msg->getSubtype() == api::network::NetPingEnable) {
				bool enabled = static_cast<api::network::Network_PingEnable_Update *>(msg->getContent())->enable;
				_manager->enablePing(enabled);
			}
		} else {
			ISIXE_THROW_MESSAGE("NetworkMailbox", "unsupported Messagetype detected: " << msg->getMessageInfo());
		}
	}

	// from Network
	void NetworkMailbox::NetworkNews(const api::GameMessage::Ptr & msg) const {
		if (msg->getMessageType() == api::messages::NetworkMessageType) {
			if (msg->getSubtype() == api::network::NetPingRequest) {
				// Got a ping request, send the answer
				api::network::Network_Ping_Update * p = new api::network::Network_Ping_Update(msg->getContent()->_sender, dynamic_cast<api::network::Network_Ping_Update *>(msg->getContent())->time);
				api::GameMessage::Ptr m = boost::make_shared<api::GameMessage>(api::messages::NetworkMessageType, api::network::NetPingAnswer, core::Method::Update, p, core::Subsystem::Network);
				_manager->publish(PINGANSWER_CHANNEL, m);
			} else if (msg->getSubtype() == api::network::NetPingAnswer) {
				if (dynamic_cast<api::network::Network_Ping_Update *>(msg->getContent())->receiver != api::EngineController::GetSingleton().getNetworkFacade()->getIP()) {
					return;
				}
				// Got a ping answer, send the ping-time
				uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
				uint64_t ping = (cT - dynamic_cast<api::network::Network_Ping_Update *>(msg->getContent())->time) / 2;
				api::network::Network_Pingtime_Update * p = new api::network::Network_Pingtime_Update(msg->getContent()->_sender, ping / 1000);
				api::GameMessage::Ptr m = boost::make_shared<api::GameMessage>(api::messages::GameMessageType, api::network::NetPingTime, core::Method::Update, p, core::Subsystem::Network);
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(m);

				api::GameMessage::Ptr m1 = boost::make_shared<api::GameMessage>(api::messages::NetworkMessageType, api::network::NetTimeSynchro, core::Method::Update, new api::network::Network_TimeSynchro_Update(msg->getContent()->_sender, ping, cT), core::Subsystem::Network);
				_manager->publish(PINGREQUEST_CHANNEL, m1);
			} else if (msg->getSubtype() == api::network::NetTimeSynchro) {
				// Got a time synchro message
				uint64_t ping = dynamic_cast<api::network::Network_TimeSynchro_Update *>(msg->getContent())->ping;
				uint64_t time = dynamic_cast<api::network::Network_TimeSynchro_Update *>(msg->getContent())->time;
				api::EngineController::GetSingleton().setCurrentTime(time + ping);
			} else {
#ifdef ISIXE_WITH_PROFILING
				msg->insertTimestamp("NetworkMailbox -> messaging");
#endif /* ISIXE_WITH_PROFILING */
				// deliver message if we're not the receiver
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			}
		} else {
#ifdef ISIXE_WITH_PROFILING
			msg->insertTimestamp("NetworkMailbox -> messaging");
#endif /* ISIXE_WITH_PROFILING */
			// deliver into local system
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		}
	}

} /* namespace modules */
} /* namespace i6e */
