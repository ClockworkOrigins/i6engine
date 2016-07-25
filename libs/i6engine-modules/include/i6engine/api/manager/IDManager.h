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
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_IDMANAGER_H__
#define __I6ENGINE_API_IDMANAGER_H__

#include <mutex>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace api {

	/**
	 * \brief handles IDs for GameObjects
	 */
	class ISIXE_MODULES_API IDManager {
	public:
		/**
		 * \brief default contructor
		 * starts id's from a random value
		 */
		IDManager();

		/**
		 * \brief sets start-ID and end-ID
		 */
		void setBounds(const int64_t from, const int64_t to);

		/**
		 * \brief get next ID
		 * \return the id that should be used for the next GameObject
		 */
		int64_t getID();

		/**
		 * \brief get multiple IDs
		 * \param[in] amount amount of requested IDs
		 * \return an ID where the next amount-1 IDs are free as well
		 */
		int64_t getID(const uint32_t amount);

	private:
		/**
		 * \brief id for the next GameObject
		 */
		int64_t _idNext;

		/**
		 * \brief highest id, that can be used
		 */
		int64_t _idMax;

		/**
		 * \brief mutex for locking the id
		 */
		mutable std::mutex _idMutex;

		/**
		 * \brief forbidden
		 */
		IDManager(const IDManager &) = delete;

		/**
		 * \brief forbidden
		 */
		IDManager & operator=(const IDManager &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_IDMANAGER_H__ */

/**
 * @}
 */
