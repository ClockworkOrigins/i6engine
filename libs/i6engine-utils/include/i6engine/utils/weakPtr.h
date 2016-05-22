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

#ifndef __I6ENGINE_UTILS_WEAKPTR_H__
#define __I6ENGINE_UTILS_WEAKPTR_H__

#include "i6engine/utils/sharedPtr.h"

namespace i6e {
namespace utils {

	/**
	 * \class weakPtr
	 * \brief A weak pointer observing a sharedPtr
	 */
	template<typename T, typename Base = T>
	class weakPtr {
	public:
		/**
		 * \brief default constructor
		 */
		weakPtr() : _sharedPtrWrapper(std::make_shared<sharedPtrWrapper<Base>>(nullptr)), _ptr(nullptr) {
		}

		/**
		 * \brief copy constructor copying another weak pointer
		 */
		weakPtr(const weakPtr & other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
		}

		/**
		 * \brief move constructor
		 */
		weakPtr(weakPtr && other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
			other._sharedPtrWrapper.reset();
			other._ptr = nullptr;
		}

		/**
		 * \brief constructor takes sharedPtr with same base class
		 */
		weakPtr(const sharedPtr<T, Base> & other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
		}

		/**
		 * \brief constructor takes sharedPtr with same base class
		 */
		template<typename U>
		weakPtr(const sharedPtr<U, T> & other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
		}

		/**
		 * \brief constructor takes sharedPtr with different types
		 */
		template<typename U, typename V>
		weakPtr(const sharedPtr<U, V> & other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
		}

		/**
		 * \brief destructor, removes sharedPtrWrapper if all weak and sharedPtr are removed
		 */
		~weakPtr() {
		}

		/**
		 * \brief assignment operator
		 */
		const weakPtr & operator=(const weakPtr & other) {
			if (*this == other) {
				return *this;
			}
			_sharedPtrWrapper = other._sharedPtrWrapper;
			_ptr = other._ptr;

			return *this;
		}

		/**
		 * \brief move assignment operator
		 */
		const weakPtr & operator=(weakPtr && other) {
			if (*this == other) {
				return *this;
			}
			_sharedPtrWrapper = other._sharedPtrWrapper;
			_ptr = other._ptr;
			other._sharedPtrWrapper.reset();
			other._ptr = nullptr;

			return *this;
		}

		/**
		 * \brief comparison between two weak pointer
		 */
		bool operator==(const weakPtr & other) const {
			return _ptr == other._ptr;
		}

		/**
		 * \brief comparison between weak and normal pointer
		 */
		bool operator==(const T * other) const {
			return _ptr == other;
		}

		bool operator!=(const weakPtr & other) const {
			return !(*this == other);
		}

		bool operator!=(const T * other) const {
			return !(*this == other);
		}

		bool operator<(const weakPtr & other) const {
			return _ptr < other._ptr;
		}

		/**
		 * \brief returns the shared pointer being observed by this weakPtr
		 */
		sharedPtr<T, Base> get() const {
			auto p = _sharedPtrWrapper.lock();
			return sharedPtr<T, Base>((p == nullptr) ? nullptr : _ptr, p);
		}

		/**
		 * \brief return true, if pointer is invalid, otherwise false
		 */
		bool expired() const {
			return _sharedPtrWrapper.expired();
		}

	private:
		std::weak_ptr<sharedPtrWrapper<Base>> _sharedPtrWrapper;
		T * _ptr;
	};

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_WEAKPTR_H__ */

/**
 * @}
 */
