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

namespace i6engine {
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
		weakPtr() : _sharedCounter(new sharedCounter()), _ptr(nullptr) {
			_sharedCounter->weakCounter++;
		}

		/**
		 * \brief copy constructor copying another weak pointer
		 */
		weakPtr(const weakPtr & other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			_sharedCounter->weakCounter++;
		}

		/**
		 * \brief move constructor
		 */
		weakPtr(weakPtr && other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			other._sharedCounter = nullptr;
			other._ptr = nullptr;
		}

		/**
		 * \brief constructor takes sharedPtr with same base class
		 */
		weakPtr(const sharedPtr<T, Base> & other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			_sharedCounter->weakCounter++;
		}

		/**
		 * \brief constructor takes sharedPtr with same base class
		 */
		template<typename U>
		weakPtr(const sharedPtr<U, T> & other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			_sharedCounter->weakCounter++;
		}

		/**
		 * \brief constructor takes sharedPtr with different types
		 */
		template<typename U, typename V>
		weakPtr(const sharedPtr<U, V> & other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			_sharedCounter->weakCounter++;
		}

		/**
		 * \brief destructor, removes sharedCounter if all weak and sharedPtr are removed
		 */
		~weakPtr() {
			removeRef();
		}

		/**
		 * \brief assignment operator
		 */
		const weakPtr & operator=(const weakPtr & other) {
			if (*this == other) {
				return *this;
			}
			removeRef();

			_sharedCounter = other._sharedCounter;
			_ptr = other._ptr;
			_sharedCounter->weakCounter++;

			return *this;
		}

		/**
		 * \brief move assignment operator
		 */
		const weakPtr & operator=(weakPtr && other) {
			if (*this == other) {
				return *this;
			}
			removeRef();

			_sharedCounter = other._sharedCounter;
			_ptr = other._ptr;
			other._sharedCounter = nullptr;
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
			if (_sharedCounter->refCounter != 0) {
				return sharedPtr<T, Base>(_ptr, _sharedCounter);
			} else {
				return sharedPtr<T, Base>(nullptr, _sharedCounter);
			}
		}

		/**
		 * \brief return true, if pointer is invalid, otherwise false
		 */
		bool expired() const {
			return _sharedCounter->refCounter == 0;
		}

	private:
		sharedCounter * _sharedCounter;
		T * _ptr;

		/**
		 * \brief handles deletion of a reference
		 */
		void removeRef() {
			if (_sharedCounter != nullptr && _sharedCounter->weakCounter-- == 1) {
				if (_sharedCounter->refCounter == 0) {
					delete _sharedCounter;
				}
			}
		}
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_WEAKPTR_H__ */

/**
 * @}
 */
