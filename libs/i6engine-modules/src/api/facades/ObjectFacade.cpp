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

#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	ObjectFacade::ObjectFacade() : _GOMap(), _lock(), _notifyNewID(), _notifyDeletedID(), _addTicker(), _removeTicker(), _loadLevelLock(), _loadLevelCondVar(), _templateList() {
	}

	ObjectFacade::~ObjectFacade() {
		_GOMap.clear();
	}

	void ObjectFacade::cleanUpAll() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjClean, core::Method::Delete, new objects::Object_Clean_Delete(), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	GOPtr ObjectFacade::getObject(const int64_t goid) const {
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _GOMap.find(goid);
		if (it != _GOMap.end()) {
			return it->second;
		}
		// Not found, return nullptr
		return GOPtr();
	}

	std::vector<GOPtr> ObjectFacade::getAllObjectsOfType(const std::string & types) const {
		return getAllObjectsOfType(utils::split(types, ";"));
	}

	std::vector<GOPtr> ObjectFacade::getAllObjectsOfType(const std::vector<std::string> & types) const {
		std::vector<GOPtr> result;

		_lock.lock();
		auto copy = _GOMap;
		_lock.unlock();
		// Iterate through _GOMap
		for (std::unordered_map<int64_t, GOPtr>::const_iterator it = copy.begin(); it != copy.end(); ++it) {
			// Wanted GameObject found
			for (const std::string & type : types) {
				if (it->second->getType() == type) {
					result.push_back(it->second);
					break;
				}
			}
		}

		return result;
	}

	std::unordered_map<int64_t, GOPtr> ObjectFacade::getGOMap() const {
		std::lock_guard<std::mutex> lg(_lock);
		return _GOMap;
	}

	void ObjectFacade::deleteAllObjectsOfType(const std::string & type) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjType, core::Method::Delete, new objects::Object_Type_Delete(type), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::createObject(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint64_t uuid, const bool sender) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectMessageType, objects::ObjCreate, core::Method::Create, new objects::Object_Create_Create(-1, gTemplate, EngineController::GetSingletonPtr()->getNetworkFacade()->getIP(), uuid, tmpl, sender), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::loadLevel(const std::string & file, const std::string & flags, const std::string & resourcesFile, const std::function<void(uint16_t)> & callback) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjLevel, core::Method::Create, new objects::Object_Level_Create(file, flags, callback, [this]() {
			std::unique_lock<std::mutex> ul(_loadLevelLock);
			_loadLevelCondVar.notify_all();
		}), core::Subsystem::Unknown);
		std::unique_lock<std::mutex> ul(_loadLevelLock);
		if (resourcesFile.empty()) {
			// object subsystem will set it too, but we are faster here!
			callback(50);
			i6eMessagingFacade->deliverMessage(msg);
		} else {
			i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraLoadResources, core::Method::Create, new graphics::Graphics_LoadResources_Create(resourcesFile, callback, msg), core::Subsystem::Unknown));
		}
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

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::createGO(const std::string & gTemplate, const objects::GOTemplate & tmpl, uint64_t uuid, const bool sender, const std::function<void(GOPtr)> & func) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectMessageType, objects::ObjCreateAndCall, core::Method::Create, new objects::Object_CreateAndCall_Create(-1, gTemplate, EngineController::GetSingletonPtr()->getNetworkFacade()->getIP(), uuid, tmpl, sender, func), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::createComponent(int64_t goid, int64_t coid, const std::string & component, const attributeMap & params) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComCreate, core::Method::Create, new components::Component_Create_Create(goid, coid, core::IPKey(), component, params), core::Subsystem::Unknown));
	}

	void ObjectFacade::createComponentCallback(int64_t goid, int64_t coid, const std::string & component, const attributeMap & params, const std::function<void(ComPtr)> & callback) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComCreateCallback, core::Method::Create, new components::Component_CreateCallback_Create(goid, coid, core::IPKey(), component, params, callback), core::Subsystem::Unknown));
	}

	void ObjectFacade::updateGOMap(const std::unordered_map<int64_t, GOPtr> & GOMap) {
		std::lock_guard<std::mutex> lg(_lock);
		_GOMap = GOMap;
	}

	void ObjectFacade::sendConditionalMessage(const GameMessage::Ptr & m, const std::function<bool(const GOPtr &)> & f, bool sync, uint32_t compFamID) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjConditionalMessage, core::Method::Update, new objects::Object_ConditionalMessage_Update(m, f, sync, compFamID), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::registerNotifyCallback(const std::function<void(int64_t)> & f) {
		_notifyNewID = f;
	}

	void ObjectFacade::registerDeletedNotifyCallback(const std::function<void(int64_t)> & f) {
		_notifyDeletedID = f;
	}

	void ObjectFacade::notifyNewID(int64_t id) {
		if (_notifyNewID != nullptr) {
			_notifyNewID(id);
		}
	}

	void ObjectFacade::notifyDeletedID(int64_t id) {
		if (_notifyDeletedID != nullptr) {
			_notifyDeletedID(id);
		}
	}

	void ObjectFacade::registerAddTickerCallback(const std::function<void(const WeakComPtr &)> & func) {
		_addTicker = func;
	}

	void ObjectFacade::registerRemoveTickerCallback(const std::function<void(int64_t)> & func) {
		_removeTicker = func;
	}

	void ObjectFacade::resetSubSystem() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::pause() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjPause, core::Method::Update, new objects::Object_Pause_Update(true), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::unpause() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjPause, core::Method::Update, new objects::Object_Pause_Update(false), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ObjectFacade::allowComponentsTicking(bool allowTicking) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::ObjectManagerMessageType, objects::ObjComponentTicking, core::Method::Update, new objects::Object_ComponentTicking_Update(allowTicking), core::Subsystem::Unknown));
	}

} /* namespace modules */
} /* namespace i6e */
