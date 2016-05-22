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

/**
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUIMULTICOLUMNLIST_H__
#define __I6ENGINE_MODULES_GUIMULTICOLUMNLIST_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class Window;
	class MultiColumnList;
} /* namespace CEGUI */

namespace i6e {
namespace modules {

	class GUIMultiColumnList : public api::GUIWidget {
	public:
		/**
		 * \brief constructor
		 */
		GUIMultiColumnList(const std::string & name, const std::string & type);

		/**
		 * \brief destructor
		 */
		~GUIMultiColumnList();

		/**
		 * \brief receives messages
		 */
		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) override;

	private:

		/**
		 * \brief adds a row to the Scoreboard
		 */
		void addRow();

		/**
		 * \brief removes all rows from the Scoreboard
		 */
		void removeRows();

		/**
		 * \brief adds an entry to a specified pair of row and column
		 */
		void addRowEntry(const uint32_t row, const uint32_t column, const std::string & entry);

		void addColumn(const std::string & columnHeader, uint32_t column, double width);

		/**
		 * \brief CEGUI type representing the MultiColumnList
		 */
		CEGUI::MultiColumnList * _list;
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUIMULTICOLUMNLIST_H__ */

/**
 * @}
 */
