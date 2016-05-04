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

#ifndef __I6ENGINE_UTILS_FUTURE_H__
#define __I6ENGINE_UTILS_FUTURE_H__

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>

namespace i6e {
namespace utils {

	/**
	 * \class Future
	 * \brief class for returning a result before it is ready
	 */
	template<typename T>
	class Future {
	public:
		/**
		 * \brief constructor
		 */
		Future() : _value(), _finished(false), _lockValue(), _valueCondVar() {
		}

		/**
		 * \brief returns the contained value
		 * if value wasn't set, this call blocks until it is set
		 */
		T get() const {
			if (!_finished) {
				std::unique_lock<std::mutex> ul(_lockValue);
				_valueCondVar.wait(ul);
			}
			return _value;
		}

		/**
		 * \brief sets value for this variable, notifies waiting get if existing
		 */
		void push(T value) {
			_value = value;
			_finished = true;
			std::unique_lock<std::mutex> ul(_lockValue);
			_valueCondVar.notify_one();
		}

	private:
		T _value;
		std::atomic<bool> _finished;
		mutable std::mutex _lockValue;
		mutable std::condition_variable _valueCondVar;
	};

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_FUTURE_H__ */

/**
 * @}
 */
