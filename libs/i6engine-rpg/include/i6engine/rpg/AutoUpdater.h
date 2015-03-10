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
		 * \brief Registers a callback for autoUpdate.
		 * \param name Name of the object to be updated.
		 * \param func Function to be called when the variable change.
		 */
		void registerUpdate(const std::string & name, const std::function<void(std::string, T)> & func) {
			_functions.push_back(std::bind(func, name, std::placeholders::_1));
		}

		/**
		 * \brief Getter for the variable.
		 * \return value.
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
		 * \brief returns value as the datatype it really is
		 */
		operator T() {
			return _value;
		}

	 private:
		/**
		 * \brief value of this variable
		 */
		T _value;

		/**
		 * \brief Vector containing all Methods.
		 */
		std::vector<std::function<void(const T)>> _functions;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_AUTOUPDATER_H__ */

/**
 * @}
 */
