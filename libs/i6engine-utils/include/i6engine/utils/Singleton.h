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

#ifndef __I6ENGINE_UTILS_SINGLETON_H__
#define __I6ENGINE_UTILS_SINGLETON_H__

#include "boost/interprocess/sync/scoped_lock.hpp"
#include "boost/make_shared.hpp"
#include "boost/thread/mutex.hpp"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_SINGLETON_H__ */

/**
 * @}
 */
