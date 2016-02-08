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

#ifndef __I6ENGINE_UTILS_AUTOUPDATER_H__
#define __I6ENGINE_UTILS_AUTOUPDATER_H__

#include <functional>
#include <string>
#include <vector>

#include "i6eSystemParameters.h"

namespace i6engine {
namespace utils {

	template<typename T>
	class AutoUpdater {
	public:
		/**
		 * \brief default constructor
		 */
		AutoUpdater() : _value(T()), _functions() {
		}

		/**
		 * \brief Constructor for a new AutoUpdate variable.
		 * \param value initial value of this variable.
		 */
		AutoUpdater(const T & value) : _value(value), _functions() {
		}

		/**
		 * \brief Registers a callback for autoUpdate being called on every change of the value
		 * \param func function to be called when the variable changes
		 */
		void registerUpdate(const std::function<void(T)> & func) {
			_functions.push_back(func);
		}

		/**
		 * \brief Getter for the value
		 * \return value
		 */
		T get() const {
			return _value;
		}

		/**
		 * \brief Setter for the value.
		 * \param val value to be set.
		 */
		void set(const T & val) {
			_value = val;
			for (std::function<void(T)> & method : _functions) {
				method(_value);
			}
		}

		/**
		 * \brief assignment operator
		 */
		T & operator=(const T & val) {
			set(val);
			return _value;
		}

		/**
		 * \brief += operator
		 */
		AutoUpdater<T> & operator+=(const T & val) {
			set(_value + val);
			return *this;
		}

		/**
		 * \brief -= operator
		 */
		AutoUpdater<T> & operator-=(const T & val) {
			set(_value - val);
			return *this;
		}

		/**
		 * \brief *= operator
		 */
		AutoUpdater<T> & operator*=(const T & val) {
			set(_value * val);
			return *this;
		}

		/**
		 * \brief /= operator
		 */
		AutoUpdater<T> & operator/=(const T & val) {
			set(_value / val);
			return *this;
		}

		/**
		 * \brief returns value as the datatype it really is
		 */
		operator T() const {
			return _value;
		}
		
		/**
		 * doesn't return another AutoUpdater, but the old value of the AutoUpdater!
		 */
		T operator++() {
			*this += 1;
			return _value;
		}

		T operator++(int) {
			T tmp = _value;
			*this += 1;
			return tmp;
		}

	 private:
		/**
		 * \brief value of this variable
		 */
		T _value;

		/**
		 * \brief Vector containing all Methods.
		 */
		std::vector<std::function<void(T)>> _functions;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_AUTOUPDATER_H__ */

/**
 * @}
 */
