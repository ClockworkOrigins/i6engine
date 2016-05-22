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

#ifndef __I6ENGINE_UTILS_SINGLETON_H__
#define __I6ENGINE_UTILS_SINGLETON_H__

#include "boost/interprocess/sync/scoped_lock.hpp"
#include "boost/make_shared.hpp"
#include "boost/thread/mutex.hpp"

namespace i6e {
namespace utils {

	/**
	 * \class Singleton
	 * \brief Derive from this templated class to make a class a singleton.
	 *		 Refer to the Singleton Design Pattern if it is not clear what a singleton is.
	 *		 Declare the ctor private but destructor public (because of destruction is handled by shared_ptr)
	 *		 T = type of new Singleton class
	 *		 Dependencies = dependencies of this Singleton, will be initialized before this Singleton is initialized and destroyed after this one is destroyed
	 */
	template<typename T, typename... Dependencies>
	class Singleton {
	public:
		/**
		 * \brief Returns a pointer to the Singleton class
		 */
		static inline T * GetSingletonPtr() {
			return Init().get();
		}

		/**
		 * \brief Returns a reference to the Singleton class
		 */
		static inline T & GetSingleton() {
			return *Init();
		}

	protected:
		Singleton() {
		}

		virtual ~Singleton() {
		}

	private:
		/**
		 * \brief helper struct for dependency initialization, called if no arguments are left
		 */
		template<typename... B>
		struct InitDependencies {
			InitDependencies() {
			}
			static void Init() {
			}
		};

		/**
		 * \brief helper struct for dependency initialization, called if arguments are left
		 */
		template<typename A, typename... B>
		struct InitDependencies<A, B...> {
			InitDependencies() {
				Init();
			}
			~InitDependencies() {
			}
			static void Init() {
				A::GetSingleton();
				InitDependencies<B...>::Init();
			}
		};

		/**
		 * \brief initializes dependencies of the Singleton and the new Singleton, returns reference to the Singleton
		 */
		static boost::shared_ptr<T> & Init() {
			static const InitDependencies<Dependencies...> id;
			static boost::shared_ptr<T> _ptrSingleton = boost::shared_ptr<T>(new T());
			return _ptrSingleton;
		}

		Singleton(const Singleton &) = delete;

		const Singleton & operator=(const Singleton &) = delete;
	};

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_SINGLETON_H__ */

/**
 * @}
 */
