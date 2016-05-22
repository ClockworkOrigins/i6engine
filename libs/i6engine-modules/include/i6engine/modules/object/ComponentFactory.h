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
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_MODULES_COMPONENTFACTORY_H__
#define __I6ENGINE_MODULES_COMPONENTFACTORY_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eThreadSafety.h"
#include "i6engine/utils/weakPtr.h"

#include "i6engine/api/configs/attributeMap.h"

#include "boost/function.hpp"

namespace i6e {
namespace api {
	class Component;
	class GameObject;

	typedef utils::sharedPtr<Component, Component> ComPtr;
	typedef utils::weakPtr<GameObject> WeakGOPtr;

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

		/**
		 * \brief sets whether newly created Components shall be allowed to tick or not
		 * default value is true
		 */
		void enableTicking(bool allowTicking) {
			_tickingAllowed = allowTicking;
		}

	private:
		typedef std::map<std::string, api::createGOCCallback> callbackMap;

		/**
		 * \brief Map of all Create methods held by the ComponentFactory.
		 * \see registerCTemplate()
		 * \see createGOCCallback
		 */
		callbackMap _callbacks;

		/**
		 * \brief stores whether new Components are allowed to tick or not
		 */
		bool _tickingAllowed;

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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTFACTORY_H__ */

/**
 * @}
 */
