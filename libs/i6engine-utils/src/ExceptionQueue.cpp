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

#include "i6engine/utils/ExceptionQueue.h"

namespace i6e {
namespace utils {
namespace exceptions {

	ExceptionQueue::ExceptionQueue() : _queue(), _mutex(), _callbacks() {
	}

	void ExceptionQueue::enqueue(const loginfo & data) {
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.push(data);
		for (auto & cb : _callbacks) {
			cb();
		}
	}

	bool ExceptionQueue::isEmpty() const {
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.empty();
	}

	loginfo ExceptionQueue::dequeue() {
		std::lock_guard<std::mutex> lock(_mutex);
		loginfo e = _queue.front();
		_queue.pop();
		return e;
	}

	void ExceptionQueue::addCallback(const std::function<void(void)> & callback) {
		std::lock_guard<std::mutex> lock(_mutex);
		_callbacks.push_back(callback);
	}

} /* namespace exceptions */
} /* namespace utils */
} /* namespace i6e */
