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

#include "i6engine/modules/gui/guiwidgets/GUIComboBox.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace modules {

	GUIComboBox::GUIComboBox(const std::string & name, const std::string & style) : api::GUIWidget(name), _selectedCallback() {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setProperty("RiseOnClickEnabled", "False");
		dynamic_cast<CEGUI::Combobox *>(_window)->setReadOnly(true);

		_window->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUIComboBox::selectionChanged, this));
	}

	void GUIComboBox::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiAddText) {
			api::gui::GUI_Text * g = dynamic_cast<api::gui::GUI_Text *>(data);
			dynamic_cast<CEGUI::Combobox *>(_window)->addItem(new CEGUI::ListboxTextItem(g->text));
		} else if (type == api::gui::GuiSetSelectedStringCallback) {
			api::gui::GUI_SetSelectedStringCallback * g = dynamic_cast<api::gui::GUI_SetSelectedStringCallback *>(data);
			_selectedCallback = g->callback;
		} else if (api::gui::GuiSetText) {
			std::string selection = dynamic_cast<api::gui::GUI_Text *>(data)->text;
			for (size_t i = 0; i < dynamic_cast<CEGUI::Combobox *>(_window)->getItemCount(); i++) {
				CEGUI::ListboxTextItem * lti = dynamic_cast<CEGUI::ListboxTextItem *>(dynamic_cast<CEGUI::Combobox *>(_window)->getListboxItemFromIndex(i));
				if (lti->getText() == selection) {
					lti->setSelected(true);
					dynamic_cast<CEGUI::Combobox *>(_window)->setText(selection);
					break;
				}
			}
		} else {
			GUIWidget::update(type, data);
		}
	}

	bool GUIComboBox::selectionChanged(const CEGUI::EventArgs & evt) {
		if (_selectedCallback == nullptr) {
			return true;
		}
		_selectedCallback(dynamic_cast<CEGUI::ListboxTextItem *>(dynamic_cast<CEGUI::Combobox *>(_window)->getSelectedItem())->getText().c_str());
		return true;
	}

} /* namespace modules */
} /* namespace i6engine */
