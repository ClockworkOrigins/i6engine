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
#include <memory>
#include <mutex>
#include <new>

namespace i6engine {
namespace utils {

	/**
	 * \brief struct handling threadsafe destruction of the wrapped pointer
	 */
	template<typename T>
	struct sharedPtrWrapper {
		T * ptr;

		sharedPtrWrapper(T * p) : ptr(p) {
		}
		~sharedPtrWrapper();
	};

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
		friend struct sharedPtrWrapper<T>;

	public:
		/**
		 * \brief default constructor creating a sharedPtr containing a nullptr
		 */
		sharedPtr() : _sharedPtrWrapper(nullptr), _ptr(nullptr) {
		}

		/**
		 * \brief normal constructor taking pointer to object to be stored
		 */
		explicit sharedPtr(T * ptr) : _sharedPtrWrapper(nullptr), _ptr(ptr) {
			_sharedPtrWrapper = std::make_shared<sharedPtrWrapper<U>>(ptr);
		}

		/**
		 * \brief copy constructor sharing reference and incrementing refCounter
		 */
		sharedPtr(const sharedPtr & other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
		}

		/**
		 * \brief move constructor
		 */
		sharedPtr(sharedPtr && other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
			other._sharedPtrWrapper = nullptr;
			other._ptr = nullptr;
		}

		/**
		 * \brief upcast derived => base
		 */
		template<typename V>
		sharedPtr(const sharedPtr<V, U> & other) : _sharedPtrWrapper(other._sharedPtrWrapper), _ptr(other._ptr) {
		}

		/**
		 * \brief destructor, adding pointer of object to clearList if last reference was deleted
		 */
		~sharedPtr() {
		}

		/**
		 * \brief assignment operator
		 */
		const sharedPtr & operator=(const sharedPtr & other) {
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
		const sharedPtr & operator=(sharedPtr && other) {
			if (*this == other) {
				return *this;
			}
			_sharedPtrWrapper = other._sharedPtrWrapper;
			_ptr = other._ptr;
			other._sharedPtrWrapper = nullptr;
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
			_sharedPtrWrapper = other._sharedPtrWrapper;
			_ptr = other._ptr;

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
			std::lock_guard<std::mutex> lg(sharedPtr<U, U>::clearListLock);
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
		std::shared_ptr<sharedPtrWrapper<U>> _sharedPtrWrapper;
		T * _ptr;

		/**
		 * \brief this constructor is called by dynamic_pointer_cast
		 */
		sharedPtr(T * ptr, std::shared_ptr<sharedPtrWrapper<U>> sC) : _sharedPtrWrapper(sC), _ptr(ptr) {
		}

		static std::mutex clearListLock;
		static std::list<U *> clearList;
	};

	template<typename T, typename U> std::mutex sharedPtr<T, U>::clearListLock;
	template<typename T, typename U> std::list<U *> sharedPtr<T, U>::clearList;

	template<typename T>
	sharedPtrWrapper<T>::~sharedPtrWrapper() {
		std::lock_guard<std::mutex> lg(sharedPtr<T, T>::clearListLock);
		sharedPtr<T, T>::clearList.push_back(reinterpret_cast<T *>(ptr));
	}

	/**
	 * \brief casts a sharedPtr of dynamic type T2 to dynamic type T1
	 */
	template<typename T1, typename T2, typename U>
	sharedPtr<T1, U> dynamic_pointer_cast(const sharedPtr<T2, U> & base) {
		T1 * t1 = dynamic_cast<T1 *>(base._ptr);
		if (t1 != nullptr) {
			return sharedPtr<T1, U>(t1, base._sharedPtrWrapper);
		} else {
			return sharedPtr<T1, U>();
		}
	}

	/**
	 * \brief creates a new sharedPtr with dynamic type T and static type U, Args are constructor arguments
	 */
	template<typename T, typename U, typename... Args>
	sharedPtr<T, U> make_shared(Args && ... args) {
		/*char * memory = static_cast<char *>(::operator new(sizeof(T) + sizeof(sharedPtrWrapper)));
		T * ptr = new(memory) T(args...);
		sharedPtrWrapper * sC = new(memory + sizeof(T)) sharedPtrWrapper();*/
		T * ptr = new T(args...);
		std::shared_ptr<sharedPtrWrapper<U>> sC = std::make_shared<sharedPtrWrapper<U>>(ptr);
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
