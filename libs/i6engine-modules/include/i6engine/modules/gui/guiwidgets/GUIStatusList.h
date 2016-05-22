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

#ifndef __I6ENGINE_MODULES_GUISTATUSLIST_H__
#define __I6ENGINE_MODULES_GUISTATUSLIST_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class Listbox;
} /* namespace CEGUI */

namespace i6e {
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
		 * \brief Sets the Lifetime
		 */
		void setLifetime(const int64_t lifetime);

		/**
		 * \brief Removes all Entries from this List.
		 */
		void clearEntries();

		bool selectionChanged(const CEGUI::EventArgs & evt);

		uint64_t _startTime;
		int64_t _lifetime;
		CEGUI::Listbox * _listbox;
		uint32_t _amount;
		std::function<void(std::string)> _selectedCallback;
		bool _autoLineBreak;
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUISTATUSLIST_H__ */

/**
 * @}
 */
