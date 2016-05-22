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

#ifndef __I6ENGINE_UTILS_AUTOUPDATER_H__
#define __I6ENGINE_UTILS_AUTOUPDATER_H__

#include <functional>
#include <string>
#include <vector>

#include "i6eSystemParameters.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_AUTOUPDATER_H__ */

/**
 * @}
 */
