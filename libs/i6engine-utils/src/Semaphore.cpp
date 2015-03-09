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

#include "i6engine/utils/Semaphore.h"

namespace i6engine {
namespace utils {

	Semaphore::Semaphore() : _mutex(), _condition(), _count(1) {
	}

	void Semaphore::notify() {
		std::unique_lock<std::mutex> lock(_mutex);
		++_count;
		_condition.notify_one();
	}

	void Semaphore::wait() {
		std::unique_lock<std::mutex> lock(_mutex);
		while (!_count) {
			_condition.wait(lock);
		}
		--_count;
	}

} /* namespace utils */
} /* namespace i6engine */
