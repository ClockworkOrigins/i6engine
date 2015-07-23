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
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONQUEUE_H__
#define __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONQUEUE_H__

#include <functional>
#include <mutex>
#include <queue>
#include <vector>

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Singleton.h"

namespace i6engine {
namespace utils {
namespace exceptions {

	/**
	 * \class ExceptionQueue
	 * \brief Implements a queue that supports multiple producers but only one consumer.
	 *
	 * Implements a queue that supports multiple producers but only one consumer for exceptions.
	 */
	class ISIXE_UTILS_API ExceptionQueue : public Singleton<ExceptionQueue> {
		friend class Singleton<ExceptionQueue>;

	public:
		/**
		 * \brief Enqueues data.
		 *
		 * \param[in] data Data to be queued
		 */
		void enqueue(const loginfo & data);

		/**
		 * \brief Returns true if the queue is empty.
		 *
		 * \return True if the queue is empty
		 */
		bool isEmpty() const;

		/**
		 * \brief Removes the first element in the queue.
		 */
		loginfo dequeue();

		/**
		 * \brief add callback to queue being called when a new entry is pushed
		 */
		void addCallback(const std::function<void(void)> & callback);

	private:
		std::queue<loginfo> _queue;
		mutable std::mutex _mutex;
		std::vector<std::function<void(void)>> _callbacks;

		/**
		 * \brief constructor
		 */
		ExceptionQueue();
	};

} /* namespace exceptions */
} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONQUEUE_H__ */

/**
 * @}
 */
