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

#ifndef __I6ENGINE_UTILS_DOUBLEBUFFERQUEUE_H__
#define __I6ENGINE_UTILS_DOUBLEBUFFERQUEUE_H__

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "boost/thread/mutex.hpp"

namespace i6e {
namespace utils {

	/**
	 * class DoubleBufferQueue
	 *
	 * T defines the data type being contained in the queue
	 * producer tells whether more than one thread pushes data into the queue
	 * consumer tells whether more than one thread pulls data from the queue
	 */
	template<typename T, bool producer = true, bool consumer = true>
	class DoubleBufferQueue {
	private:
		template<bool v>
		struct Bool2Type {
			enum {
				value = v
			};
		};

	public:
		/**
		 * \brief default constructor
		 */
		DoubleBufferQueue() : _queueA(), _queueB(), _queueRead(&_queueA), _queueWrite(&_queueB), _readLock(), _writeLock() {
		}

		/**
		 * \brief pushes the given value into the queue
		 */
		void push(const T & value) {
			boost::mutex::scoped_lock scopeLock(_writeLock);
			_queueWrite->push(value);
		}

		/**
		 * \brief removes first entry of the queue
		 */
		void pop() {
			pop(Bool2Type<consumer>());
		}

		/**
		 * \brief returns first entry of the queue
		 */
		T front() {
			return front(Bool2Type<consumer>());
		}

		/**
		 * \brief removes first entry of the queue and returns its value
		 */
		T poll() {
			return poll(Bool2Type<consumer>());
		}

		/**
		 * \brief returns true if the queue is empty, otherwise false
		 */
		inline bool empty() const {
			return _queueRead->empty() && _queueWrite->empty();
		}

		/**
		 * \brief returns size of the queue
		 */
		inline size_t size() const {
			return _queueRead->size() + _queueWrite->size();
		}

		/**
		 * \brief removes all elements in the queue
		 */
		void clear() {
			_readLock.lock();
			while (!_queueRead->empty()) {
				_queueRead->pop();
			}
			_readLock.unlock();
			_writeLock.lock();
			while (!_queueWrite->empty()) {
				_queueWrite->pop();
			}
			_writeLock.unlock();
		}

	private:
		/**
		 * \brief the two queues containing the read and write data
		 */
		std::queue<T> _queueA;
		std::queue<T> _queueB;

		/**
		 * \brief pointers to the real queues, switched in swap
		 */
		std::queue<T> * _queueRead;
		std::queue<T> * _queueWrite;

		boost::mutex _readLock;
		boost::mutex _writeLock;

		void pop(Bool2Type<true>) {
			static_assert(consumer, "Consumer must be true here");
			boost::mutex::scoped_lock scopeLock(_readLock);
			if (_queueRead->empty()) {
				swap();

				if (_queueRead->empty()) {
					ISIXE_THROW_API("DoubleBufferQueue", "nothing to pop");
				}
			}

			_queueRead->pop();
		}

		void pop(Bool2Type<false>) {
			static_assert(!consumer, "Consumer must be false here");
			if (_queueRead->empty()) {
				swap();

				if (_queueRead->empty()) {
					ISIXE_THROW_API("DoubleBufferQueue", "nothing to pop");
				}
			}

			_queueRead->pop();
		}

		T front(Bool2Type<true>) {
			static_assert(consumer, "Consumer must be true here");
			boost::mutex::scoped_lock scopeLock(_readLock);
			if (_queueRead->empty()) {
				swap();

				if (_queueRead->empty()) {
					ISIXE_THROW_API("DoubleBufferQueue", "nothing to get");
				}
			}
			return _queueRead->front();
		}

		T front(Bool2Type<false>) {
			static_assert(!consumer, "Consumer must be false here");
			if (_queueRead->empty()) {
				swap();

				if (_queueRead->empty()) {
					ISIXE_THROW_API("DoubleBufferQueue", "nothing to get");
				}
			}
			return _queueRead->front();
		}

		T poll(Bool2Type<true> b) {
			static_assert(consumer, "Consumer must be true here");
			boost::mutex::scoped_lock scopeLock(_readLock);
			if (_queueRead->empty()) {
				swap();

				if (_queueRead->empty()) {
					ISIXE_THROW_API("DoubleBufferQueue", "nothing to get");
				}
			}

			T ret = _queueRead->front();
			_queueRead->pop();
			return ret;
		}

		T poll(Bool2Type<false> b) {
			static_assert(!consumer, "Consumer must be false here");
			if (_queueRead->empty()) {
				swap();

				if (_queueRead->empty()) {
					ISIXE_THROW_API("DoubleBufferQueue", "nothing to get");
				}
			}

			T ret = _queueRead->front();
			_queueRead->pop();
			return ret;
		}

		/**
		 * \brief swaps read and write buffer
		 */
		void swap() {
			boost::mutex::scoped_lock scopeLock2(_writeLock);
			if (_queueRead == &_queueA) {
				_queueWrite = &_queueA;
				_queueRead = &_queueB;
			} else {
				_queueWrite = &_queueB;
				_queueRead = &_queueA;
			}
		}

		/**
		 * \brief forbidden
		 */
		DoubleBufferQueue(const DoubleBufferQueue &) = delete;
	};

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_DOUBLEBUFFERQUEUE_H__ */

/**
 * @}
 */
