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

#ifndef __I6ENGINE_UTILS_SHAREDPTR_H__
#define __I6ENGINE_UTILS_SHAREDPTR_H__

#include <atomic>
#include <list>
#include <new>

#include "boost/interprocess/sync/scoped_lock.hpp"
#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace utils {

	/**
	 * \brief struct handling counters for the references
	 * a sharedCounter that stores a nullptr doesn't have a sharedCounter
	 */
	typedef struct sharedCounter {
		std::atomic<uint32_t> refCounter;
		std::atomic<uint32_t> weakCounter;

		sharedCounter() : refCounter(0), weakCounter(0) {
		}
		~sharedCounter() {
		}
	} sharedCounter;

	template<typename T, typename U>
	class sharedPtr;

	template<typename T, typename Base>
	class weakPtr;

	template<typename T1, typename T2, typename U>
	sharedPtr<T1, U> dynamic_pointer_cast(const sharedPtr<T2, U> &);

	template<typename T, typename U, typename... Args>
	sharedPtr<T, U> make_shared(Args && ...);

	/**
	 * \class sharedPtr
	 * \brief A shared pointer counting references and adds objects being not referenced any more to an internal list
	 * required external call of clear to delete all objects, no automatic deletion to stay threadsafe
	 * T = type of the pointer, U = base type of the pointer
	 */
	template<typename T, typename U>
	class sharedPtr {
		template<typename T1, typename T2, typename U1>
		friend sharedPtr<T1, U1> dynamic_pointer_cast(const sharedPtr<T2, U1> &);
		template<typename T1, typename U2, typename... Args>
		friend sharedPtr<T1, U2> make_shared(Args && ...);
		friend class weakPtr<T, U>;
		template<typename V, typename W>
		friend class sharedPtr;
		template<typename V, typename W>
		friend class weakPtr;

	public:
		/**
		 * \brief default constructor creating a sharedPtr containing a nullptr
		 */
		sharedPtr() : _sharedCounter(nullptr), _ptr(nullptr) {
		}

		/**
		 * \brief normal constructor taking pointer to object to be stored
		 */
		explicit sharedPtr(T * ptr) : _sharedCounter(nullptr), _ptr(ptr) {
			if (ptr != nullptr) {
				_sharedCounter = new sharedCounter();
				_sharedCounter->refCounter++;
			}
		}

		/**
		 * \brief copy constructor sharing reference and incrementing refCounter
		 */
		sharedPtr(const sharedPtr & other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			if (_ptr != nullptr) {
				_sharedCounter->refCounter++;
			}
		}

		/**
		 * \brief move constructor
		 */
		sharedPtr(sharedPtr && other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			other._sharedCounter = nullptr;
			other._ptr = nullptr;
		}

		/**
		 * \brief upcast derived => base
		 */
		template<typename V>
		sharedPtr(const sharedPtr<V, U> & other) : _sharedCounter(other._sharedCounter), _ptr(other._ptr) {
			if (_ptr != nullptr) {
				_sharedCounter->refCounter++;
			}
		}

		/**
		 * \brief destructor, adding pointer of object to clearList if last reference was deleted
		 */
		~sharedPtr() {
			removeRef();
		}

		/**
		 * \brief assignment operator
		 */
		const sharedPtr & operator=(const sharedPtr & other) {
			if (*this == other) {
				return *this;
			}
			removeRef();

			_sharedCounter = other._sharedCounter;
			_ptr = other._ptr;
			if (_ptr != nullptr) {
				_sharedCounter->refCounter++;
			}

			return *this;
		}

		/**
		 * \brief move assignment operator
		 */
		const sharedPtr & operator=(sharedPtr && other) {
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
		 * \brief upcast, derived => base
		 */
		template<typename V>
		const sharedPtr & operator=(const sharedPtr<V, U> & other) {
			if (*this == other) {
				return *this;
			}
			removeRef();

			_sharedCounter = other._sharedCounter;
			_ptr = other._ptr;
			if (_ptr != nullptr) {
				_sharedCounter->refCounter++;
			}

			return *this;
		}

		/**
		 * \brief -> operator returning stored pointer
		 */
		T * operator->() const {
			return _ptr;
		}

		/**
		 * \brief comparison between two shared pointer
		 */
		bool operator==(const sharedPtr & other) const {
			return _ptr == other._ptr;
		}

		/**
		 * \brief comparison between shared and normal pointer
		 */
		bool operator==(const T * other) const {
			return _ptr == other;
		}

		friend bool operator==(const T * other, const sharedPtr & self) {
			return self == other;
		}

		bool operator!=(const sharedPtr & other) const {
			return !(*this == other);
		}

		friend bool operator!=(const T * other, const sharedPtr & self) {
			return self != other;
		}

		bool operator!=(const T * other) const {
			return !(*this == other);
		}

		/**
		 * \brief deletes all pointers in clearList
		 */
		static void clear() {
			boost::mutex::scoped_lock l(sharedPtr<U, U>::clearListLock);
			for (U * ptr : sharedPtr<U, U>::clearList) {
				delete ptr;
			}
			sharedPtr<U, U>::clearList.clear();
		}

		/**
		 * \brief returns the contained pointer
		 */
		T * get() const {
			return _ptr;
		}

	private:
		/**
		 * \brief this constructor is called by dynamic_pointer_cast
		 */
		sharedPtr(T * ptr, sharedCounter * sC) : _sharedCounter(sC), _ptr(ptr) {
			_sharedCounter->refCounter++;
		}

		/**
		 * \brief handles deletion of a reference
		 */
		void removeRef() {
			if (_sharedCounter != nullptr) {
				if (_sharedCounter->refCounter-- == 1) {
					{
						boost::mutex::scoped_lock l(sharedPtr<U, U>::clearListLock);
						sharedPtr<U, U>::clearList.push_back(reinterpret_cast<U *>(_ptr));
					}

					if (_sharedCounter->weakCounter == 0) {
						delete _sharedCounter;
						_sharedCounter = nullptr;
					}
				}
			}
		}

		sharedCounter * _sharedCounter;
		T * _ptr;

		static boost::mutex clearListLock;
		static std::list<U *> clearList;
	};

	template<typename T, typename U> boost::mutex sharedPtr<T, U>::clearListLock;
	template<typename T, typename U> std::list<U *> sharedPtr<T, U>::clearList;

	/**
	 * \brief casts a sharedPtr of dynamic type T2 to dynamic type T1
	 */
	template<typename T1, typename T2, typename U>
	sharedPtr<T1, U> dynamic_pointer_cast(const sharedPtr<T2, U> & base) {
		T1 * t1 = dynamic_cast<T1 *>(base._ptr);
		if (t1 != nullptr) {
			return sharedPtr<T1, U>(t1, base._sharedCounter);
		} else {
			return sharedPtr<T1, U>();
		}
	}

	/**
	 * \brief creates a new sharedPtr with dynamic type T and static type U, Args are constructor arguments
	 */
	template<typename T, typename U, typename... Args>
	sharedPtr<T, U> make_shared(Args && ... args) {
		/*char * memory = static_cast<char *>(::operator new(sizeof(T) + sizeof(sharedCounter)));
		T * ptr = new(memory) T(args...);
		sharedCounter * sC = new(memory + sizeof(T)) sharedCounter();*/
		T * ptr = new T(args...);
		sharedCounter * sC = new sharedCounter();
		return sharedPtr<T, U>(ptr, sC);
	}

	/**
	 * \brief gets pointer of a shared pointer
	 * necessary for luabind and boost::python
	 */
	template<class T, class U> T * get_pointer(const sharedPtr<T, U> & p) {
		return p.get();
	}

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_SHAREDPTR_H__ */

/**
 * @}
 */
