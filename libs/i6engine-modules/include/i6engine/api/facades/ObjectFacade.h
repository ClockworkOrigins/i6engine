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

#ifndef __I6ENGINE_API_OBJECTFACADE_H__
#define __I6ENGINE_API_OBJECTFACADE_H__

#include <condition_variable>
#include <list>
#include <map>
#include <mutex>

#include "i6engine/utils/weakPtr.h"

#include "i6engine/api/configs/ObjectConfig.h"

#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace api {

	class Component;
	class GameObject;

	typedef std::map<std::string, std::string> attributeMap;
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
		 * \deprecated don't use this method or don't call any function on the pointer. This 
		 * is highly thread unsafe...
		 * \param[in] GameObjectID
		 * \return A pointer to the wanted GameObject
		 */
		GOPtr getObject(const int64_t guid) const;

		/**
		 * \brief It returns _GOList.
		 * \deprecated don't use this method or don't call any function on the pointer. This 
		 * is highly thread unsafe..
		 * \return _GOList
		 */
		std::list<GOPtr> getGOList() const;

		/**
		 * \brief returns a vector with all GameObjects of the given type
		 * \param[in] type the list of types given in one string, using ; as delimiter
		 * \returns list
		 */
		std::list<GOPtr> getAllObjectsOfType(const std::string & types) const;

		/**
		 * \brief returns a vector with all GameObjects of the given type
		 * \param[in] type the list of types given as a vector
		 * \returns list
		 */
		std::list<GOPtr> getAllObjectsOfType(const std::vector<std::string> & types) const;

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
		void createObject(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint32_t uuid, const bool sender) const;

		/**
		 * \brief This method will be called when shutting down ObjectController.
		 * For every GameObject in _GOList DeleteObject will be called.
		 */
		void cleanUpAll() const;

		/**
		 * \brief loads a level from an xml file only using the objects with given flags
		 */
		void loadLevel(const std::string & file, const std::string & flags) const;

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
		 * \brief creates a new GameObject of that type and calls given callback after creation is done
		 *
		 * Calls the appropriate create-function and returns the new GameObject. It is automatically stored in the ObjectController, thus manually deleting the object shoudn't be necessary.
		 * param[in] name the string used to identify a template.
		 * param[in] params a list of pair<std::string, std::string> used to parameterize the creation process
		 * param[in] func callback to be called with the new GameObject
		 * param[in] sender true if this GameObject should be synchronized in multiplayer games
		 */
		void createGO(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint32_t uuid, const bool sender, const boost::function<void(GOPtr)> & func) const;

		/**
		 * \brief creates a new Component for given GameObject
		 * \param goid GameObject id
		 * \param coid id for the Component
		 * \param component registered string for create method
		 * \param params attributeMap for this Component
		 */
		void createComponent(int64_t goid, int64_t coid, const std::string & component, const attributeMap & params) const;

		/**
		 * \brief updates the GOList with current state in ObjectSubsystem
		 * \note only call from within object subsystem
		 */
		void updateGOList(const std::list<GOPtr> & GOList);

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
		 * \brief notifies new id in subsystem
		 */
		void notifyNewID(int64_t id);

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

	private:
		/**
		 * \brief List with all current GameObjects.
		 */
		std::list<GOPtr> _GOList;

		/**
		 * \brief for locking the _GOList
		 */
		mutable boost::mutex _lock;

		/**
		 * \brief contains the current frameTime of this subsystem
		 */
		uint32_t _frameTime;

		boost::function<void(int64_t)> _notify;

		boost::function<void(const WeakComPtr &)> _addTicker;
		boost::function<void(int64_t)> _removeTicker;

		mutable std::mutex _loadLevelLock;
		mutable std::condition_variable _loadLevelCondVar;

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
} /* namespace i6engine */

#endif /* __I6ENGINE_API_OBJECTFACADE_H__ */

/**
 * @}
 */
