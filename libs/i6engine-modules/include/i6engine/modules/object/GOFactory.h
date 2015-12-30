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

#ifndef __I6ENGINE_MODULES_GOFACTORY_H__
#define __I6ENGINE_MODULES_GOFACTORY_H__

#include <map>

#include "i6engine/utils/i6eThreadSafety.h"
#include "i6engine/utils/sharedPtr.h"

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6engine {
namespace api {
	class GameObject;
} /* namespace api */

namespace modules {

	class ComponentFactory;
	class ObjectManager;

	typedef utils::sharedPtr<api::GameObject, api::GameObject> GOPtr;

	/**
	 * \class GOFactory
	 * \brief Factory in order to create GameObjects
	 * \see api::GameObject
	 */
	class GOFactory {
	public:
		/**
		 * \brief used to easily create special GameObjects
		 *
		 * It is possible to register different GameObject at the factory with a string to identify them.<br>
		 * After registering, you can create this kind of GameObjects by just calling the create function with the ID-String
		 */
		GOFactory(ObjectManager * m, ComponentFactory * c);

		/**
		 * \brief register a new Template
		 * After a template is registered you can easily create new objects of that type with createGO()
		 *
		 * \see createGO()
		 * \param[in] file the template file containing object data
		 */
		void registerGOTemplate(const std::string & file);

		/**
		 * \brief creates a new GameObject of that type
		 *
		 * Calls the appropriate create-function and returns the new GameObject.
		 * \param[in] name the string used to identify a template.
		 * \param[in] params a list of pair<std::string, std::string> used to parameterize the creation process
		 * \param[in] sender true if this GameObject should be synchronized in multiplayer games
		 * \return pointer to a new created GameObject or NULL if no template with that name is registered.
		 */
		GOPtr createGO(const int64_t goid, const core::IPKey & owner, uint64_t uuid, const std::string & tpl, const api::objects::GOTemplate & templ, const bool sender);
		GOPtr createGO(const std::string & tmpl, int64_t goid, const core::IPKey & owner, uint64_t uuid, const api::objects::GOTemplate & templ, const bool sender);

		/**
		 * \brief loads a level out of the given file with the given flags
		 * \param[in] file file to open
		 * \param[in] flags
		 * \throws I6_FAILURE When file can't be opened
		 */
		void loadLevel(const std::string & file, const std::string & flags);

	private:
		/**
		 * \brief list of CreateGO pointers, identified by their names.
		 *
		 * \see registerGOTemplate()
		 * \see CreateGO
		 */
		std::map<std::string, api::objects::GOTemplate> _templateList;

		/**
		 * \brief ObjectManager, to insert new GOs
		 */
		ObjectManager * _manager;

		/**
		 * \brief ComponentFactory, needed for created GOs
		 */
		ComponentFactory * _compFactory;

		/**
		 * \brief forbidden
		 */
		GOFactory(const GOFactory &) = delete;

		/**
		 * \brief forbidden
		 */
		GOFactory & operator=(const GOFactory &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GOFACTORY_H__ */

/**
 * @}
 */
