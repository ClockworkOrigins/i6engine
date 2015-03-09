/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

namespace i6engine {
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
				api::network::Network_Ping_Update * p = new api::network::Network_Ping_Update(msg->getContent()->_sender, static_cast<api::network::Network_Ping_Update *>(msg->getContent())->time);
				api::GameMessage::Ptr m = boost::make_shared<api::GameMessage>(api::messages::NetworkMessageType, api::network::NetPingAnswer, core::Method::Update, p, i6engine::core::Subsystem::Network);
				_manager->publish(PINGANSWER_CHANNEL, m);
			} else if (msg->getSubtype() == api::network::NetPingAnswer) {
				// Got a ping answer, send the ping-time
				uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
				uint64_t ping = (cT - static_cast<api::network::Network_Ping_Update *>(msg->getContent())->time) / 2;
				api::network::Network_Pingtime_Update * p = new api::network::Network_Pingtime_Update(msg->getContent()->_sender, ping / 1000);
				api::GameMessage::Ptr m = boost::make_shared<api::GameMessage>(api::messages::GameMessageType, api::network::NetPingTime, core::Method::Update, p, i6engine::core::Subsystem::Network);
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(m);

				api::GameMessage::Ptr m1 = boost::make_shared<api::GameMessage>(api::messages::NetworkMessageType, api::network::NetTimeSynchro, core::Method::Update, new api::network::Network_TimeSynchro_Update(msg->getContent()->_sender, ping, cT), i6engine::core::Subsystem::Network);
				_manager->publish(PINGREQUEST_CHANNEL, m1);
			} else if (msg->getSubtype() == api::network::NetTimeSynchro) {
				// Got a time synchro message
				uint64_t ping = dynamic_cast<api::network::Network_TimeSynchro_Update *>(msg->getContent())->ping;
				uint64_t time = dynamic_cast<api::network::Network_TimeSynchro_Update *>(msg->getContent())->time;
				api::EngineController::GetSingleton().setCurrentTime(time + ping);
			} else {
#ifdef ISIXE_PROFILING
				msg->insertTimestamp("NetworkMailbox -> messaging");
#endif /* ISIXE_PROFILING */
				// deliver message if we're not the receiver
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			}
		} else {
#ifdef ISIXE_PROFILING
			msg->insertTimestamp("NetworkMailbox -> messaging");
#endif /* ISIXE_PROFILING */
			// deliver into local system
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		}
	}

} /* namespace modules */
} /* namespace i6engine */
