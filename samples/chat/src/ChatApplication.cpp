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

#include "ChatApplication.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/NetworkConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/bind.hpp"

namespace sample {

	ChatApplication::ChatApplication(const std::string & remoteIP, uint16_t remotePort, uint16_t localPort) : i6engine::api::Application(), _remoteIP(remoteIP), _remotePort(remotePort), _localPort(localPort) {
	}

	ChatApplication::~ChatApplication() {
	}

	void ChatApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6engine::api::messages::ChatMessageType, ChatApplication::News, this);
	}

	void ChatApplication::AfterInitialize() {
		if (_localPort == _remotePort) { // if port of client and host are the same this node is the host, so listen to port
			i6engine::api::EngineController::GetSingleton().getNetworkFacade()->listen(_localPort);
		} else { // otherwise this node has to connect to host
			i6engine::api::EngineController::GetSingleton().getNetworkFacade()->connect(i6engine::core::IPKey(_remoteIP, _remotePort), _localPort);
		}
		// with this subscription to the chat channel this node gets all messages on this channel
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->subscribe(CHAT_CHANNEL);

		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->addImage("Background", "RPG/StaticImage", "RPG", "MainMenu", 0.0, 0.0, 1.0, 1.0);

		// an editbox changing the text of the print
		gf->addStatusList("Listbox", "RPG/Listbox", 0.1, 0.05, -1);
		gf->setSize("Listbox", 0.8, 0.85);

		// an editbox changing the text of the print
		gf->addEditbox("Editbox", "RPG/Editbox", 0.1, 0.9, 0.4, 0.05, "", [gf](std::string s) {
		});
		// this callback is used to get the message
		gf->setAcceptedTextCallback("Editbox", [gf](std::string s) {
			// add sender to message
			s = i6engine::api::EngineController::GetSingleton().getNetworkFacade()->getIP().toString() + ": " + s;

			// add message to own list
			gf->addTextToWidget("Listbox", s);

			// clear Editbox
			gf->setText("Editbox", "");

			// send message over network to all registered clients
			i6engine::api::EngineController::GetSingleton().getNetworkFacade()->publish(CHAT_CHANNEL, boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::ChatMessageType, i6engine::api::network::ChatMessage, i6engine::core::Method::Create, new i6engine::api::network::Network_ChatMessage(s), i6engine::core::Subsystem::Unknown));

			return "";
		});

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));
	}

	void ChatApplication::Tick() {
	}

	bool ChatApplication::ShutdownRequest() {
		return true;
	}

	void ChatApplication::Finalize() {
		ISIXE_UNREGISTERMESSAGETYPE(i6engine::api::messages::ChatMessageType);
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->disconnect();
	}

	void ChatApplication::ShutDown() {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->unsubscribe(CHAT_CHANNEL);
	}

	void ChatApplication::News(const i6engine::api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == i6engine::api::messages::ChatMessageType) {
			if (msg->getSubtype() == i6engine::api::network::ChatMessage) {
				// got message from other instance, so add message to listbox
				i6engine::api::EngineController::GetSingleton().getGUIFacade()->addTextToWidget("Listbox", dynamic_cast<i6engine::api::network::Network_ChatMessage *>(msg->getContent())->text);
			}
		}
	}

} /* namespace sample */
