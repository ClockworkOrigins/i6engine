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

#include "i6engine/modules/gui/guiwidgets/GUIMultiColumnList.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace modules {

	GUIMultiColumnList::GUIMultiColumnList(const std::string & name, const std::string & style) : GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_list = dynamic_cast<CEGUI::MultiColumnList *>(_window);
	}

	GUIMultiColumnList::~GUIMultiColumnList() {
	}

	void GUIMultiColumnList::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiAddRow) {
			addRow();
		} else if (type == api::gui::GuiClearWidget) {
			removeRows();
		} else if (type == api::gui::GuiSetRowEntry) {
			api::gui::GUI_Add_Row_Entry * g = static_cast<api::gui::GUI_Add_Row_Entry *>(data);
			addRowEntry(g->row, g->column, g->entry);
		} else if (type == api::gui::GuiAddColumn) {
			api::gui::GUI_AddColumn * g = static_cast<api::gui::GUI_AddColumn *>(data);
			addColumn(g->columnHeader, g->column, g->width);
		} else {
			api::GUIWidget::update(type, data);
		}
	}

	void GUIMultiColumnList::addRow() {
		_list->addRow();
	}

	void GUIMultiColumnList::addRowEntry(const uint32_t row, const uint32_t column, const std::string & entry) {
		CEGUI::ListboxTextItem * item = new CEGUI::ListboxTextItem(entry, 0);

		_list->setItem(item, column, row);
	}

	void GUIMultiColumnList::removeRows() {
		for (CEGUI::uint i = 0; i < _list->getRowCount(); ++i) {
			_list->removeRow(i);
		}
	}

	void GUIMultiColumnList::addColumn(const std::string & columnHeader, uint32_t column, double width) {
		_list->addColumn(columnHeader, column, CEGUI::UDim(float(width), 0.0));
	}

} /* namespace modules */
} /* namespace i6engine */
