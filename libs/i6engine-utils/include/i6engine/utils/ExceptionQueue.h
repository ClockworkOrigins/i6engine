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

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONQUEUE_H__ */

/**
 * @}
 */
