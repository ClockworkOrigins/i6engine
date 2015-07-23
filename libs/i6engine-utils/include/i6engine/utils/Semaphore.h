/*
 * Copyright 2011-2012 Prof. Dr. Richard Lenz, Thomas Fischer
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
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_SEMAPHORE_H__
#define __I6ENGINE_UTILS_SEMAPHORE_H__

#include <condition_variable>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6engine {
namespace utils {

	class ISIXE_UTILS_API Semaphore {
	public:
		/**
		 * \brief default constructor
		 */
		Semaphore();

		/**
		 * \brief unlocks one thread
		 */
		void notify();

		/**
		 * \brief locks one thread
		 */
		void wait();

	private:
		mutable std::mutex _mutex;
		std::condition_variable _condition;
		uint32_t _count;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_SEMAPHORE_H__ */

/**
 * @}
 */
