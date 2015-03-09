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

#include "i6engine/editor/gui/ListboxWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "i6engine/editor/EditorMessageTypes.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace editor {
namespace gui {

	ListboxWidget::ListboxWidget(const std::string & name, const std::string & style) : GUIWidget(name), _callbacks() {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow("Editor/Listbox", name);
		_window->setRiseOnClickEnabled(false);

		_window->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&ListboxWidget::changedSelection, this));
	}

	ListboxWidget::~ListboxWidget() {
	}

	void ListboxWidget::update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetVisible) {
			bool vis = static_cast<i6engine::api::gui::GUI_Visibility *>(data)->visible;
			_window->setVisible(vis);
		} else if (type == messages::GUIMessageTypes::RemoveEntries) {
			_callbacks.clear();
			dynamic_cast<CEGUI::Listbox *>(_window)->resetList();
		} else if (type == messages::GUIMessageTypes::AddEntry) {
			messages::GUI_AddEntry * g = static_cast<messages::GUI_AddEntry *>(data);
			addEntry(g->objName);
			_callbacks.push_back(g->callback);
		} else if (type == api::gui::GuiSetPosition) {
			api::gui::GUI_Position * g = static_cast<api::gui::GUI_Position *>(data);
			setPosition(g->x, g->y);
		} else if (type == api::gui::GuiSetSize) {
			api::gui::GUI_Size * g = static_cast<api::gui::GUI_Size *>(data);
			setSize(g->width, g->height);
		} else {
			ISIXE_THROW_API("GUI", "Don't know what to do with " << type);
		}
	}

	void ListboxWidget::setPosition(double x, double y) {
		_window->setPosition(CEGUI::UVector2(CEGUI::UDim(float(x), 0.0f), CEGUI::UDim(float(y), 0.0f)));
	}

	void ListboxWidget::setSize(double w, double h) {
		_window->setSize(CEGUI::USize(CEGUI::UDim(float(w), 0.0f), CEGUI::UDim(float(h), 0.0f)));
	}

	void ListboxWidget::addEntry(const std::string & entry) {
		dynamic_cast<CEGUI::Listbox *>(_window)->addItem(new CEGUI::ListboxTextItem(entry));
	}

	bool ListboxWidget::changedSelection(const CEGUI::EventArgs & e) {
		CEGUI::ListboxItem * item = dynamic_cast<CEGUI::Listbox *>(_window)->getFirstSelectedItem();
		size_t index = dynamic_cast<CEGUI::Listbox *>(_window)->getItemIndex(item);

		_callbacks[index]();

		return true;
	}

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
