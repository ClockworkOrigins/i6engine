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

#include "i6engine/modules/gui/guiwidgets/GUIChat.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/NetworkConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIChat::GUIChat(const std::string & name, const std::string &) : api::GUIWidget(name), api::MessageSubscriberFacade(), _submitCallback() {
		loadWindowLayout(name, "Chat.layout");
		_window->setUserData(this);
		_window->setProperty("RiseOnClickEnabled", "False");

		_window->getChild("Editbox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&GUIChat::textAccepted, this));
		_window->setUsingAutoRenderingSurface(true);

		ISIXE_REGISTERMESSAGETYPE(api::messages::ChatMessageType, GUIChat::News, this);
	}

	GUIChat::~GUIChat() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ChatMessageType);
	}

	void GUIChat::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetTextAcceptCallback) {
			api::gui::GUI_SetAcceptTextCallback * g = static_cast<api::gui::GUI_SetAcceptTextCallback *>(data);
			_submitCallback = g->callback;
		} else if (type == api::gui::GuiAddText) {
			api::gui::GUI_Text * g = static_cast<api::gui::GUI_Text *>(data);
			_window->setText(g->text);
		} else {
			GUIWidget::update(type, data);
		}
	}

	bool GUIChat::textAccepted(const CEGUI::EventArgs &) {
		if (_submitCallback == nullptr) {
			return true;
		}
		std::string entry = _submitCallback(std::string(_window->getChild("Text")->getText().c_str()));
		dynamic_cast<CEGUI::Listbox *>(_window->getChild("Text"))->addItem(new CEGUI::ListboxTextItem(entry));
		api::EngineController::GetSingleton().getNetworkFacade()->publish(CHAT_CHANNEL, boost::make_shared<api::GameMessage>(api::messages::ChatMessageType, api::network::ChatMessage, core::Method::Create, new api::network::Network_ChatMessage(entry), core::Subsystem::Unknown));
		_window->getChild("Editbox")->setText("");
		return true;
	}

	void GUIChat::News(const api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == api::messages::ChatMessageType) {
			if (msg->getSubtype() == api::network::ChatMessage) {
				dynamic_cast<CEGUI::Listbox *>(_window->getChild("Text"))->addItem(new CEGUI::ListboxTextItem(dynamic_cast<api::network::Network_ChatMessage *>(msg->getContent())->text));
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
