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

#include "i6engine/modules/gui/guiwidgets/GUIStatusList.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIStatusList::GUIStatusList(const std::string & name, const std::string & type) : api::GUIWidget(name), _startTime(), _lifetime(-1), _amount(UINT32_MAX), _selectedCallback(), _autoLineBreak(false) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_listbox = dynamic_cast<CEGUI::Listbox *>(wmgr.createWindow(type, _name));

		_listbox->addItem(new CEGUI::ListboxTextItem(""));

		_window = _listbox;
	}

	GUIStatusList::~GUIStatusList() {
		enableTicking(false);
	}

	void GUIStatusList::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiAddText) {
			std::string text = dynamic_cast<api::gui::GUI_Text *>(data)->text;
			addMessage(text);
		} else if (type == api::gui::GuiSetLifetime) {
			const api::gui::GUI_Lifetime * guiP = (static_cast<api::gui::GUI_Lifetime *>(data));
			setLifetime(guiP->lifetime);
		} else if (type == api::gui::GuiClearWidget) {
			clearEntries();
		} else if (type == api::gui::GuiSetAmount) {
			_amount = static_cast<api::gui::GUI_Amount *>(data)->amount;
		} else if (type == api::gui::GuiSetSelectedStringCallback) {
			if (_selectedCallback == nullptr) {
				_window->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&GUIStatusList::selectionChanged, this));
			}
			_selectedCallback = static_cast<api::gui::GUI_SetSelectedStringCallback *>(data)->callback;
		} else if (type == api::gui::GuiSetFont) {
			dynamic_cast<CEGUI::Listbox *>(_window)->setFont(static_cast<api::gui::GUI_Text *>(data)->text);
		} else if (type == api::gui::GuiSetAutoLineBreak) {
			_autoLineBreak = static_cast<api::gui::GUI_SetAutoLineBreak *>(data)->enabled;
		} else {
			GUIWidget::update(type, data);
		}
	}

	void GUIStatusList::addMessage(const std::string & message) {
		if (_autoLineBreak) {
			auto vec = utils::split(message, " ");
			if (vec.empty()) {
				return;
			}
			CEGUI::ListboxTextItem * lbi = new CEGUI::ListboxTextItem("");
			_listbox->addItem(lbi);
			std::string last = "";
			std::string current = vec[0];
			for (size_t i = 1; i < vec.size(); i++) {
				last = current;
				current += " " + vec[i];
				lbi->setText(current);
				if (_listbox->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width >= _window->getSize().d_width.d_scale) {
					lbi->setText(last);
					lbi = new CEGUI::ListboxTextItem("");
					_listbox->addItem(lbi);
					last = "";
					current = vec[i];
				}
			}
			lbi->setText(current);
		} else {
			CEGUI::ListboxTextItem * lbi = new CEGUI::ListboxTextItem(message);
			_listbox->addItem(lbi);
		}

		_startTime = api::EngineController::GetSingleton().getCurrentTime();

		if (_listbox->getItemCount() > _amount) {
			_listbox->removeItem(_listbox->getListboxItemFromIndex(0));
		}
	}

	void GUIStatusList::setLifetime(const int64_t lifetime) {
		if (_lifetime == -1 && lifetime != -1) {
			enableTicking(true);
		}
		_lifetime = lifetime;
	}

	void GUIStatusList::clearEntries() {
		while (_listbox->getItemCount() > 0) {
			_listbox->removeItem(_listbox->getListboxItemFromIndex(0));
		}
	}

	void GUIStatusList::tick() {
		if (_lifetime == -1) {
			enableTicking(false);
			return; // -1 means infinite
		}
		if (uint64_t(int64_t(_startTime) + _lifetime) <= api::EngineController::GetSingleton().getCurrentTime()) {
			if (_listbox->getItemCount() > 0) {
				_listbox->removeItem(_listbox->getListboxItemFromIndex(0));
			}
		}
	}

	bool GUIStatusList::selectionChanged(const CEGUI::EventArgs &) {
		if (_selectedCallback == nullptr) {
			return true;
		}
		_selectedCallback(dynamic_cast<CEGUI::ListboxTextItem *>(_listbox->getFirstSelectedItem())->getText().c_str());
		return true;
	}

} /* namespace modules */
} /* namespace i6e */
