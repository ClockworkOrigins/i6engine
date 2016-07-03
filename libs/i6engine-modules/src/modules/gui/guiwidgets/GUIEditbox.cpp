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

#include "i6engine/modules/gui/guiwidgets/GUIEditbox.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIEditbox::GUIEditbox(const std::string & name, const std::string & style) : api::GUIWidget(name), _enterCallback(), _submitCallback() {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setUserData(this);
		_window->setProperty("RiseOnClickEnabled", "False");

		_window->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&GUIEditbox::textChanged, this));
	}

	void GUIEditbox::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetEnterCallback) {
			api::gui::GUI_SetEnterTextCallback * g = static_cast<api::gui::GUI_SetEnterTextCallback *>(data);
			_enterCallback = g->callback;
		} else if (type == api::gui::GuiSetTextAcceptCallback) {
			api::gui::GUI_SetAcceptTextCallback * g = static_cast<api::gui::GUI_SetAcceptTextCallback *>(data);
			_submitCallback = g->callback;
			_window->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&GUIEditbox::textAccepted, this));
		} else if (type == api::gui::GuiSetText) {
			api::gui::GUI_Text * g = static_cast<api::gui::GUI_Text *>(data);
			_window->setText(g->text);
		} else {
			GUIWidget::update(type, data);
		}
	}

	bool GUIEditbox::textChanged(const CEGUI::EventArgs &) {
		if (_enterCallback == nullptr) {
			return true;
		}
		_enterCallback(std::string(_window->getText().c_str()));
		return true;
	}

	bool GUIEditbox::textAccepted(const CEGUI::EventArgs &) {
		if (_submitCallback == nullptr) {
			return true;
		}
		std::string entry = _submitCallback(std::string(_window->getText().c_str()));
		return true;
	}

} /* namespace modules */
} /* namespace i6e */
