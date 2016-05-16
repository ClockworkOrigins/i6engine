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

		_window->setUsingAutoRenderingSurface(true);

		CEGUI::RenderingSurface* rs = _window->getRenderingSurface();
		if (rs) {
			rs->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueEnded, CEGUI::Event::Subscriber(&api::GUIWidget::renderingEndedHandler, dynamic_cast<api::GUIWidget *>(this)));
		}
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
