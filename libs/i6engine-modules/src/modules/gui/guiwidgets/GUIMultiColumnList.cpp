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

#include "i6engine/modules/gui/guiwidgets/GUIMultiColumnList.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/WindowManager.h"
#include "CEGUI/widgets/ListboxTextItem.h"
#include "CEGUI/widgets/MultiColumnList.h"

namespace i6e {
namespace modules {

	GUIMultiColumnList::GUIMultiColumnList(const std::string & name, const std::string & style) : GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setUserData(this);
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
} /* namespace i6e */
