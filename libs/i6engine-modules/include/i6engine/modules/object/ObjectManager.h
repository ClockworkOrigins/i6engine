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

#ifndef __I6ENGINE_MODULES_OBJECTMANAGER_H__
#define __I6ENGINE_MODULES_OBJECTMANAGER_H__

#include "i6engine/utils/sequence_map.h"
#include "i6engine/utils/sharedPtr.h"

#include "i6engine/modules/object/ComponentFactory.h"
#include "i6engine/modules/object/GOFactory.h"

namespace i6engine {
namespace api {
	typedef utils::weakPtr<Component> WeakComPtr;
} /* namespace api */
namespace modules {

	class ObjectMailbox;

	typedef utils::sharedPtr<api::GameObject, api::GameObject> GOPtr;

	class ObjectManager {
		friend class GOFactory;
		friend class ObjectMailbox;

	public:
		/**
		 * \brief constructor, initializes Object stuff
		 */
		ObjectManager();

		/**
		 * \brief destructor
		 */
		~ObjectManager();

		/**
		 * \brief tick of the ObjectManager, called by ObjectController to inform ObjectManager there is the next frame
		 */
		void Tick();

		/**
		 * \brief registers a template with callback for creating a Component
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
		api::ComPtr createGOC(const int64_t id, const std::string & GOCType, const api::attributeMap & params, const GOPtr & owner) const;

		/**
		 * \brief loads a level out of the given file with the given flags
		 * \param[in] file file to open
		 * \param[in] flags
		 * \throws I6_FAILURE When file can't be opened
		 */
		void loadLevel(const std::string & file, const std::string & flags);

	private:
		/**
		 * \brief register a new Template
		 * After a template is registered you can easily create new objects of that type with createGO()
		 *
		 * \see createGO()
		 * \param[in] name the string used to identify this Template. The name must be unique.
		 * \param[in] fn the CreateGO function, which creates the GameObject
		 */
		void registerGOTemplate(const std::string & file);

		/**
		 * \brief List with all current GameObjects.
		 */
		std::list<GOPtr> _GOList;

		utils::sequence_map<int64_t, api::WeakComPtr> _tickList;

		ComponentFactory _componentFactory;
		GOFactory _goFactory;

		/**
		 * \brief is the subsystem paused or not?
		 */
		bool _paused;

		/**
		 * \brief Handles incoming create messages for GameObjects and the ObjectManager
		 */
		void NewsCreate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief Handles incomming update messages for the ObjectManager
		 */
		void NewsUpdate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief Handles incomming delete messages for GameObjects and the ObjectManager
		 */
		void NewsDelete(const api::GameMessage::Ptr & msg);

		/**
		 * \brief This method will be called in every tick of the subsystem and will process all the messages of
		 * all game object components.
		 */
		void tickComponents();

		/**
		 * \brief Adds a component to the list of objects to be ticked
		 */
		void addTicker(const api::WeakComPtr & c);

		/**
		 * \brief Removes a component from the list of objects to be ticked
		 */
		void removeTicker(int64_t c);

		/**
		 * \brief sends all stored messages of all objects to the given receiver for synchronization
		 * \param [in] receiver Receiver who requested the Message
		 */
		void sendObjectMessages(const core::IPKey & receiver) const;

		/**
		 * \brief Deletes given GameObject
		 */
		void deleteObject(int64_t id);

		/**
		 * \brief Inserts an object to the list
		 */
		void insertObject(const GOPtr & go);

		/**
		 * \brief deletes all objects with the given type
		 */
		void deleteAllObjectsOfType(const std::string & type);

		/**
		 * \brief It returns a pointer to the GameObject with GUID guid.
		 * If there is no GameObject with GUID guid, NULL is returned.
		 * \param[in] GameObjectID
		 * \return A pointer to the wanted GameObject
		 */
		GOPtr getObject(const int64_t guid) const;

		/**
		 * \brief delivers the given message to all GameObjects fullfilling the given condition function
		 */
		void sendConditionalMessage(const api::objects::Object_ConditionalMessage_Update & msg);

		/**
		 * \brief forbidden
		 */
		ObjectManager(const ObjectManager &) = delete;

		/**
		 * \brief forbidden
		 */
		ObjectManager & operator=(const ObjectManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_OBJECTMANAGER_H__ */

/**
 * @}
 */
