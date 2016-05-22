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

	ChatApplication::ChatApplication(const std::string & remoteIP, uint16_t remotePort, uint16_t localPort) : i6e::api::Application(), _remoteIP(remoteIP), _remotePort(remotePort), _localPort(localPort) {
	}

	ChatApplication::~ChatApplication() {
	}

	void ChatApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6e::api::messages::ChatMessageType, ChatApplication::News, this);
	}

	void ChatApplication::AfterInitialize() {
		if (_localPort == _remotePort) { // if port of client and host are the same this node is the host, so listen to port
			i6e::api::EngineController::GetSingleton().getNetworkFacade()->listen(_localPort);
		} else { // otherwise this node has to connect to host
			i6e::api::EngineController::GetSingleton().getNetworkFacade()->connect(i6e::core::IPKey(_remoteIP, _remotePort), _localPort);
		}
		// with this subscription to the chat channel this node gets all messages on this channel
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->subscribe(CHAT_CHANNEL);

		i6e::api::GUIFacade * gf = i6e::api::EngineController::GetSingleton().getGUIFacade();

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
			s = i6e::api::EngineController::GetSingleton().getNetworkFacade()->getIP().toString() + ": " + s;

			// add message to own list
			gf->addTextToWidget("Listbox", s);

			// clear Editbox
			gf->setText("Editbox", "");

			// send message over network to all registered clients
			i6e::api::EngineController::GetSingleton().getNetworkFacade()->publish(CHAT_CHANNEL, boost::make_shared<i6e::api::GameMessage>(i6e::api::messages::ChatMessageType, i6e::api::network::ChatMessage, i6e::core::Method::Create, new i6e::api::network::Network_ChatMessage(s), i6e::core::Subsystem::Unknown));

			return "";
		});

		// register ESC to close the application
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, boost::bind(&i6e::api::EngineController::stop, i6e::api::EngineController::GetSingletonPtr()));
	}

	void ChatApplication::Tick() {
	}

	bool ChatApplication::ShutdownRequest() {
		return true;
	}

	void ChatApplication::Finalize() {
		ISIXE_UNREGISTERMESSAGETYPE(i6e::api::messages::ChatMessageType);
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->disconnect();
	}

	void ChatApplication::ShutDown() {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->unsubscribe(CHAT_CHANNEL);
	}

	void ChatApplication::News(const i6e::api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == i6e::api::messages::ChatMessageType) {
			if (msg->getSubtype() == i6e::api::network::ChatMessage) {
				// got message from other instance, so add message to listbox
				i6e::api::EngineController::GetSingleton().getGUIFacade()->addTextToWidget("Listbox", dynamic_cast<i6e::api::network::Network_ChatMessage *>(msg->getContent())->text);
			}
		}
	}

} /* namespace sample */
