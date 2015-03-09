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

#ifndef __I6ENGINE_MODULES_GUISTATUSLIST_H__
#define __I6ENGINE_MODULES_GUISTATUSLIST_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class Listbox;
}

namespace i6engine {
namespace modules {

	class GUIStatusList : public api::GUIWidget {
	public:
		/**
		 * \brief Creates a print with given values
		 *
		 * \param name Name of the window
		 * \param type Type of the window
		 */
		GUIStatusList(const std::string & name, const std::string & type);

		/**
		 * \brief destructor
		 */
		~GUIStatusList();

		/**
		 * \brief Message handler
		 */
		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) override;

		/**
		 * \brief Getter for the amount of ticks needed to be deleted
		 */

		/**
		 * \brief Decreases ticks (neccessary for auto-hiding)
		 *
		 * \return   nothing
		 *
		 */
		void tick() override;

	private:
		/**
		 * \brief forbidden
		 */
		GUIStatusList(const GUIStatusList &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIStatusList & operator=(const GUIStatusList &) = delete;

		/**
		 * \brief Adds a message to the StatusList
		 *
		 * \param message to add
		 */
		void addMessage(const std::string & message);

		/**
		 * \brief Sets the position of the GUIStatusList
		 */
		void setPosition(const double x, const double y);

		/**
		 * \brief Sets the Lifetime
		 */
		void setLifetime(const int64_t lifetime);

		/**
		 * \brief Removes all Entries from this List.
		 */
		void clearEntries();

		uint64_t _startTime;
		int64_t _lifetime;
		CEGUI::Listbox * lb;
		uint32_t _amount;
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUISTATUSLIST_H__ */

/**
 * @}
 */
