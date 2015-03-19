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
 * \addtogroup Manager
 * @{
 */

#ifndef __I6ENGINE_API_IDMANAGER_H__
#define __I6ENGINE_API_IDMANAGER_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace api {

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
		mutable boost::mutex _idMutex;

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
} /* namespace i6engine */

#endif /* __I6ENGINE_API_IDMANAGER_H__ */

/**
 * @}
 */
