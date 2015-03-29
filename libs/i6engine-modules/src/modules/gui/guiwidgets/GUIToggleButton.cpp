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

#include "i6engine/modules/gui/guiwidgets/GUIToggleButton.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace modules {

	GUIToggleButton::GUIToggleButton(const std::string & name, const std::string & style) : api::GUIWidget(name), _selectedCallback() {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setProperty("RiseOnClickEnabled", "False");

		_window->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GUIToggleButton::selectionChanged, this));
	}

	void GUIToggleButton::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetSelected) {
			api::gui::GUI_SetSelected * g = dynamic_cast<api::gui::GUI_SetSelected *>(data);
			dynamic_cast<CEGUI::ToggleButton *>(_window)->setSelected(g->selected);
		} else if (type == api::gui::GuiSetSelectedCallback) {
			api::gui::GUI_SetSelectedCallback * g = dynamic_cast<api::gui::GUI_SetSelectedCallback *>(data);
			_selectedCallback = g->callback;
		} else {
			GUIWidget::update(type, data);
		}
	}

	bool GUIToggleButton::selectionChanged(const CEGUI::EventArgs & evt) {
		if (_selectedCallback == nullptr) {
			return true;
		}
		_selectedCallback(dynamic_cast<CEGUI::ToggleButton *>(_window)->isSelected());
		return true;
	}

} /* namespace modules */
} /* namespace i6engine */