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

#include "i6engine/editor/gui/MenuBarWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "i6engine/editor/EditorMessageTypes.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace editor {
namespace gui {

	MenuBarWidget::MenuBarWidget(const std::string & name, const std::string & style) : GUIWidget(name), _menus(), _callbacks() {
		loadWindowLayout(name, "MenuBar.layout");
		_window->setRiseOnClickEnabled(false);

		_menus["File"] = dynamic_cast<CEGUI::Combobox *>(_window->getChild("File"));

		_menus["File"]->setText("File");
		_menus["File"]->setReadOnly(true);

		_menus["File"]->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&MenuBarWidget::clickedFileMenu, this));
	}

	MenuBarWidget::~MenuBarWidget() {
	}

	void MenuBarWidget::update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) {
		if (type == messages::GUIMessageTypes::AddMenuBarEntry) {
			messages::GUI_MenuBarEntry * mbe = dynamic_cast<messages::GUI_MenuBarEntry *>(data);

			_menus[mbe->menu]->addItem(new CEGUI::ListboxTextItem(mbe->submenu));
			_callbacks.insert(std::make_pair(std::make_pair(mbe->menu, mbe->submenu), mbe->callback));
		} else {
			GUIWidget::update(type, data);
		}
	}

	bool MenuBarWidget::clickedFileMenu(const CEGUI::EventArgs & e) {
		_callbacks[std::make_pair("File", std::string(_menus["File"]->getText().c_str()))]();
		_menus["File"]->setText("File");
		return true;
	}

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
