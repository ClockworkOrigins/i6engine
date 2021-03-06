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

#ifndef __I6ENGINE_API_OBJECTFACADE_H__
#define __I6ENGINE_API_OBJECTFACADE_H__

#include <condition_variable>
#include <map>
#include <mutex>
#include <unordered_map>

#include "i6engine/utils/weakPtr.h"

#include "i6engine/api/configs/attributeMap.h"
#include "i6engine/api/configs/ObjectConfig.h"

namespace i6e {
namespace api {

	class Component;
	class GameObject;

	typedef utils::sharedPtr<GameObject, GameObject> GOPtr;
	typedef utils::weakPtr<Component> WeakComPtr;

	class ISIXE_MODULES_API ObjectFacade {
	public:
		/**
		 * \brief Constructor for ObjectFacade.
		 */
		ObjectFacade();

		~ObjectFacade();

		/**
		 * \brief It returns a pointer to the GameObject with GUID guid.
		 * If there is no GameObject with GUID guid, NULL is returned.
		 * \param[in] GameObjectID
		 * \return A pointer to the wanted GameObject
		 */
		GOPtr getObject(const int64_t guid) const;

		/**
		 * \brief It returns _GOList.
		 * \return _GOList
		 */
		std::unordered_map<int64_t, GOPtr> getGOMap() const;

		/**
		 * \brief returns a vector with all GameObjects of the given type
		 * \param[in] types the list of types given in one string, using ; as delimiter
		 * \returns list
		 */
		std::vector<GOPtr> getAllObjectsOfType(const std::string & types) const;

		/**
		 * \brief returns a vector with all GameObjects of the given type
		 * \param[in] types the list of types given as a vector
		 * \returns list
		 */
		std::vector<GOPtr> getAllObjectsOfType(const std::vector<std::string> & types) const;

		/**
		 * \brief deletes all Objects of a given type
		 */
		void deleteAllObjectsOfType(const std::string & type) const;

		/**
		 * \brief creates a new Object
		 * \param[in] gTemplate template name
		 * \param[in] params parameter map
		 * \param[in] sender whether this GameObject should have a NetworkSenderComponent
		 */
		void createObject(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint64_t uuid, const bool sender) const;

		/**
		 * \brief For every GameObject in _GOMap deleteObject will be called.
		 */
		void cleanUpAll() const;

		/**
		 * \brief loads a level from an xml file only using the objects with given flags
		 */
		void loadLevel(const std::string & file, const std::string & flags) const {
			loadLevel(file, flags, "", [](uint16_t) {});
		}

		/**
		 * \brief loads a level from an xml file only using the objects with given flags and preloads resources specified in resourcesFile
		 */
		void loadLevel(const std::string & file, const std::string & flags, const std::string & resourcesFile) const {
			loadLevel(file, flags, resourcesFile, [](uint16_t) {});
		}
		
		/**
		 * \brief loads a level from an xml file only using the objects with given flags
		 * calls callback when progress changes with current value and maximum value
		 * the first fifty percent are always the preload of the resources (in this case instant finished), the second fifty percent are the real level
		 */
		void loadLevel(const std::string & file, const std::string & flags, const std::function<void(uint16_t)> & callback) const {
			loadLevel(file, flags, "", callback);
		}

		/**
		 * \brief loads a level from an xml file only using the objects with given flags and preloads resources specified in resourcesFile
		 * calls callback when progress changes with current value and maximum value
		 * the first fifty percent are always the preload of the resources, the second fifty percent are the real level
		 */
		void loadLevel(const std::string & file, const std::string & flags, const std::string & resourcesFile, const std::function<void(uint16_t)> & callback) const;

		/**
		 * \brief registers a component to be ticked in every Frame
		 */
		void addTicker(const WeakComPtr & c);

		/**
		 * \brief unregisters the component
		 */
		void removeTicker(int64_t c);

		/**
		 * \brief returns duration of one frame of the Object SubSystem
		 */
		uint32_t getFrameTime() const;

		/**
		 * \brief Registers the given Create method and the GOCType in the callback map of the factory.
		 * \param[in] GOCType of type std::string
		 * \param[in] fn of type createGOCCallback
		 * \return true if registration was succesful
		 */
		void registerCTemplate(const std::string & GOCType, const createGOCCallback & fn) const;

		/**
		 * \brief creates a new GameObject of that type and calls given callback from object thread after creation is done
		 *
		 * Calls the appropriate create-function and returns the new GameObject. It is automatically stored in the ObjectManager, thus manually deleting the object shouldn't be necessary.
		 * param[in] name the string used to identify a template.
		 * param[in] params a list of pair<std::string, std::string> used to parameterize the creation process
		 * param[in] func callback to be called with the new GameObject
		 * param[in] sender true if this GameObject should be synchronized in multiplayer games
		 */
		void createGO(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint64_t uuid, const bool sender, const boost::function<void(GOPtr)> & func) const;

		/**
		 * \brief creates a new Component for given GameObject
		 * \param goid GameObject id
		 * \param coid id for the Component
		 * \param component registered string for create method
		 * \param params attributeMap for this Component
		 */
		void createComponent(int64_t goid, int64_t coid, const std::string & component, const attributeMap & params) const;

		/**
		 * \brief creates a new Component for given GameObject, calls callback from object thread when Component is created and initialized
		 * \param goid GameObject id
		 * \param coid id for the Component
		 * \param component registered string for create method
		 * \param params attributeMap for this Component
		 * \param callback callback that is called after Component is created and initialized
		 */
		void createComponentCallback(int64_t goid, int64_t coid, const std::string & component, const attributeMap & params, const boost::function<void(ComPtr)> & callback) const;

		/**
		 * \brief updates the GOMap with current state in ObjectSubsystem
		 * \note only call from within object subsystem
		 */
		void updateGOMap(const std::unordered_map<int64_t, GOPtr> & GOMap);

		/**
		 * \brief sets the frametime for this subsystem in the facade, so you can see it from outside the subsystem
		 */
		void setFrametime(uint32_t frametime) { _frameTime = frametime; }

		/**
		 * \brief sends the given message to all GameObjects fullfilling the given condition function
		 * \param m Message to send
		 * \param f condition function
		 * \param sync true iff messages should go to all clients
		 * \param compFamID sends the message to the component of this family ID if the GameObject has it. If this is empty the message is send to the GameObject.
		 */
		void sendConditionalMessage(const GameMessage::Ptr & m, const boost::function<bool(const GOPtr &)> & f, bool sync, uint32_t compFamID) const;

		/**
		 * \brief register callback for notifying IDs, call only from ObjectController
		 */
		void registerNotifyCallback(const boost::function<void(int64_t)> & f);

		/**
		 * \brief register callback for notifying deleted IDs, call only from ObjectController
		 */
		void registerDeletedNotifyCallback(const boost::function<void(int64_t)> & f);

		/**
		 * \brief notifies new id in subsystem
		 */
		void notifyNewID(int64_t id);

		/**
		 * \brief notifies deleted id in subsystem
		 */
		void notifyDeletedID(int64_t id);

		/**
		 * \brief registers callback for addTicker, call only by Object Subsystem
		 */
		void registerAddTickerCallback(const boost::function<void(const WeakComPtr &)> & func);

		/**
		 * \brief registers callback for removeTicker, call only by Object Subsystem
		 */
		void registerRemoveTickerCallback(const boost::function<void(int64_t)> & func);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem() const;

		/**
		 * \brief pauses the subsystem
		 */
		void pause() const;

		/**
		 * \brief unpauses the subsystem
		 */
		void unpause() const;

		/**
		 * \brief sets all available GOTemplates
		 * don't use, only for Object Subsystem
		 */
		void setGOTemplates(const std::map<std::string, objects::GOTemplate> & templates) {
			_templateList = templates;
		}

		/**
		 * \brief returns all available GOTemplates
		 */
		std::map<std::string, objects::GOTemplate> getGOTemplates() const {
			return _templateList;
		}

		/**
		 * \brief sets whether newly created Components shall be able to tick per default or not
		 * per default Components ticking is allowed, but some cases like the level editor want to disable ticking
		 */
		void allowComponentsTicking(bool allowTicking) const;

	private:
		/**
		 * \brief List with all current GameObjects.
		 */
		std::unordered_map<int64_t, GOPtr> _GOMap;

		/**
		 * \brief for locking the _GOMap
		 */
		mutable std::mutex _lock;

		/**
		 * \brief contains the current frameTime of this subsystem
		 */
		uint32_t _frameTime;

		boost::function<void(int64_t)> _notifyNewID;
		boost::function<void(int64_t)> _notifyDeletedID;

		boost::function<void(const WeakComPtr &)> _addTicker;
		boost::function<void(int64_t)> _removeTicker;

		mutable std::mutex _loadLevelLock;
		mutable std::condition_variable _loadLevelCondVar;

		std::map<std::string, objects::GOTemplate> _templateList;

		/**
		 * \brief forbidden
		 */
		ObjectFacade(const ObjectFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		ObjectFacade & operator=(const ObjectFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_OBJECTFACADE_H__ */

/**
 * @}
 */
