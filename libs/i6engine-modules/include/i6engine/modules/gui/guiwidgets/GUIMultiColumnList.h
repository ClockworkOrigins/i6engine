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

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUIMULTICOLUMNLIST_H__ */

/**
 * @}
 */
