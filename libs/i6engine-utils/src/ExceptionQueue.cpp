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

#include "i6engine/utils/ExceptionQueue.h"

namespace i6engine {
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
} /* namespace exceptions */
