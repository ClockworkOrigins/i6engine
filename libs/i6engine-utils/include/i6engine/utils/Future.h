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
