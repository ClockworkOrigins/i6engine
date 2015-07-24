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
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_MODULES_COMPONENTFACTORY_H__
#define __I6ENGINE_MODULES_COMPONENTFACTORY_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eThreadSafety.h"
#include "i6engine/utils/weakPtr.h"

#include "boost/function.hpp"

namespace i6engine {
namespace api {
	class Component;
	class GameObject;

	typedef utils::sharedPtr<Component, Component> ComPtr;
	typedef utils::weakPtr<GameObject> WeakGOPtr;
	typedef std::map<std::string, std::string> attributeMap;

	/**
	 * A pointer to a function which creates a new Components
	 * \see Components
	 */
	typedef boost::function<ComPtr(const int64_t, const attributeMap &)> createGOCCallback;
} /* namespace api */

namespace modules {

	/**
	 * \class ComponentFactory
	 * \brief Factory in order to create Component
	 * \see api::Component
	 */
	class ComponentFactory {
	public:
		/**
		 * \brief used to easily create special Components
		 *
		 * It is possible to register different Components at the factory with a string to identify them.<br>
		 * After registering, you can create this kind of Component by just calling the create function with the ID-String
		 */
		ComponentFactory();

		/**
		 * \brief Destructor for ComponentsFactory. Clears the map of Create methods.
		 */
		~ComponentFactory();

		/**
		 * \brief register a new Template
		 * After a template is registered you can easily create new objects of that type with createGOC()
		 *
		 * \see createGOCCallback()
		 * \param[in] name the string used to identify this Template. The name must be unique.
		 * \param[in] fn the createGOCCallback function, which creates the Component
		 */
		void registerCTemplate(const std::string & GOCType, const api::createGOCCallback & fn);

		/**
		 * \brief Creates a new Component
		 *
		 * \param[in] GOCType name of the Component to create.
		 * \param[in] params a std::map<std::string, std::string> used to parameterize the creation process
		 * \return a pointer to the created GOComponent
		 * \throw I6_FAILURE When GOCType is not registered at this factory.
		 */
		api::ComPtr createGOC(int64_t id, const std::string & GOCType, const api::attributeMap & params, const api::WeakGOPtr & owner) const;

	private:
		typedef std::map<std::string, api::createGOCCallback> callbackMap;

		/**
		 * \brief Map of all Create methods held by the ComponentFactory.
		 * \see registerCTemplate()
		 * \see createGOCCallback
		 */
		callbackMap _callbacks;

		/**
		 * \brief forbidden
		 */
		ComponentFactory(const ComponentFactory &) = delete;

		/**
		 * \brief forbidden
		 */
		ComponentFactory & operator=(const ComponentFactory &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COMPONENTFACTORY_H__ */

/**
 * @}
 */
