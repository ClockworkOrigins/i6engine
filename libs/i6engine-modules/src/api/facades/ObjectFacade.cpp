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

#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/interprocess/sync/scoped_lock.hpp"

namespace i6engine {
namespace api {

	ObjectFacade::ObjectFacade() : _GOList(), _lock(), _notify(), _addTicker(), _removeTicker(), _loadLevelLock(), _loadLevelCondVar() {
	}

	ObjectFacade::~ObjectFacade() {
		_GOList.clear();
	}

	void ObjectFacade::cleanUpAll() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjClean, core::Method::Delete, new objects::Object_Clean_Delete(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	GOPtr ObjectFacade::getObject(const int64_t goid) const {
		boost::mutex::scoped_lock sl(_lock);
		// Iterate through _GOList
		for (std::list<GOPtr>::const_iterator it = _GOList.begin(); it != _GOList.end(); ++it) {
			// Wanted GameObject found
			if ((*it)->getID() == goid) {
				// Return a pointer to the wanted GameObject
				return *it;
			}
		}

		// Not found, return nullptr
		return GOPtr();
	}

	std::list<GOPtr> ObjectFacade::getAllObjectsOfType(const std::string & types) const {
		return getAllObjectsOfType(utils::split(types, ";"));
	}

	std::list<GOPtr> ObjectFacade::getAllObjectsOfType(const std::vector<std::string> & types) const {
		std::list<GOPtr> all;

		boost::mutex::scoped_lock sl(_lock);
		// Iterate through _GOList
		for (std::list<GOPtr>::const_iterator it = _GOList.begin(); it != _GOList.end(); ++it) {
			// Wanted GameObject found
			for (const std::string & type : types) {
				if ((*it)->getType() == type) {
					all.push_back(*it);
					break;
				}
			}
		}

		return all;
	}

	std::list<GOPtr> ObjectFacade::getGOList() const {
		boost::mutex::scoped_lock sl(_lock);
		return _GOList;
	}

	void ObjectFacade::deleteAllObjectsOfType(const std::string & type) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjType, core::Method::Delete, new objects::Object_Type_Delete(type), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::createObject(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint32_t uuid, const bool sender) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectMessageType, objects::ObjCreate, core::Method::Create, new objects::Object_Create_Create(-1, gTemplate, EngineController::GetSingletonPtr()->getNetworkFacade()->getIP(), uuid, tmpl, sender), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::loadLevel(const std::string & file, const std::string & flags) const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjLevel, core::Method::Create, new objects::Object_Level_Create(file, flags, [this]() {
			_loadLevelCondVar.notify_all();
		}), core::Subsystem::Unknown));
		std::unique_lock<std::mutex> ul(_loadLevelLock);
		_loadLevelCondVar.wait(ul);
	}

	void ObjectFacade::addTicker(const WeakComPtr & c) {
		_addTicker(c);
	}

	void ObjectFacade::removeTicker(int64_t c) {
		_removeTicker(c);
	}

	uint32_t ObjectFacade::getFrameTime() const {
		return _frameTime;
	}

	void ObjectFacade::registerCTemplate(const std::string & GOCType, const createGOCCallback & fn) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjRegisterCTemplate, core::Method::Create, new objects::Object_RegisterCTemplate_Create(GOCType, fn), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::createGO(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint32_t uuid, const bool sender, const boost::function<void(GOPtr)> & func) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectMessageType, objects::ObjCreateAndCall, core::Method::Create, new objects::Object_CreateAndCall_Create(-1, gTemplate, EngineController::GetSingletonPtr()->getNetworkFacade()->getIP(), uuid, tmpl, sender, func), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::updateGOList(const std::list<GOPtr> & GOList) {
		boost::mutex::scoped_lock sl(_lock);
		_GOList = GOList;
	}

	void ObjectFacade::sendConditionalMessage(const GameMessage::Ptr & m, const boost::function<bool(const GOPtr &)> & f, bool sync, uint32_t compFamID) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjConditionalMessage, core::Method::Update, new objects::Object_ConditionalMessage_Update(m, f, sync, compFamID), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::registerNotifyCallback(const boost::function<void(int64_t)> & f) {
		_notify = f;
	}

	void ObjectFacade::notifyNewID(int64_t id) {
		if (!_notify.empty()) {
			_notify(id);
		}
	}

	void ObjectFacade::registerAddTickerCallback(const boost::function<void(const WeakComPtr &)> & func) {
		_addTicker = func;
	}

	void ObjectFacade::registerRemoveTickerCallback(const boost::function<void(int64_t)> & func) {
		_removeTicker = func;
	}

	void ObjectFacade::resetSubSystem() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::pause() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjPause, core::Method::Update, new objects::Object_Pause_Update(true), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ObjectFacade::unpause() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjPause, core::Method::Update, new objects::Object_Pause_Update(false), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace modules */
} /* namespace i6engine */
