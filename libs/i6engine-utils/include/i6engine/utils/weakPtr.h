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
