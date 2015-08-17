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

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#define __STDC_LIMIT_MACROS
	#include <stdint.h>
#endif

#include "i6engine/modules/object/ObjectManager.h"

#include "i6engine/utils/Exceptions.h"

#ifdef ISIXE_WITH_PROFILING
	#include "i6engine/utils/Profiling.h"
#endif

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/i6eComponentRegister.h" // generated by cmake containing list of all Components to register
#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/NetworkConfig.h"
#include "i6engine/api/configs/ObjectConfig.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/bind.hpp"

namespace i6engine {
namespace modules {

	ObjectManager::ObjectManager() : _GOList(), _tickList(), _componentFactory(), _goFactory(this, &_componentFactory), _paused(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		api::EngineController::GetSingletonPtr()->getObjectFacade()->registerAddTickerCallback(boost::bind(&ObjectManager::addTicker, this, _1));
		api::EngineController::GetSingletonPtr()->getObjectFacade()->registerRemoveTickerCallback(boost::bind(&ObjectManager::removeTicker, this, _1));

		for (std::pair<std::string, api::createGOCCallback> & p : api::componentList) {
			registerCTemplate(p.first, p.second);
		}
	}

	ObjectManager::~ObjectManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_tickList.clear();

		_GOList.clear();
		GOPtr::clear();
		api::ComPtr::clear();
	}

	void ObjectManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (!_paused) {
			tickComponents();
		}

		GOPtr::clear();
		api::ComPtr::clear();

#ifdef ISIXE_WITH_PROFILING
		if (utils::profiling::numberObjects) {
			ISIXE_LOG_INFO("ObjectManager", "Current amount of GameObjects: " << _GOList.size());
		}
#endif /* ISIXE_PROFLING */
	}

	void ObjectManager::registerCTemplate(const std::string & GOCType, const api::createGOCCallback & fn) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_componentFactory.registerCTemplate(GOCType, fn);
	}

	void ObjectManager::registerGOTemplate(const std::string & file) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_goFactory.registerGOTemplate(file);
	}

	api::ComPtr ObjectManager::createGOC(const int64_t id, const std::string & GOCType, const api::attributeMap & params, const GOPtr & owner) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		return _componentFactory.createGOC(id, GOCType, params, owner);
	}

	void ObjectManager::loadLevel(const std::string & file, const std::string & flags) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_goFactory.loadLevel(file, flags);
	}

	void ObjectManager::NewsCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (msg->getMessageType() == api::messages::ObjectMessageType) {
			// Message to create a GameObject
			if (type == api::objects::ObjCreate) {
				int64_t goid = msg->getContent()->getID();

				api::objects::Object_Create_Create * occ = static_cast<api::objects::Object_Create_Create *>(msg->getContent());

				if (getObject(goid) != nullptr) {
					ISIXE_LOG_ERROR("ObjectManager", "Object already exists");
					return;
				}

				_goFactory.createGO(occ->tpl, occ->getID(), occ->owner, occ->uuid, occ->tmpl, occ->send);
			} else if (type == api::objects::ObjCreateAndCall) {
				api::objects::Object_CreateAndCall_Create * occ = static_cast<api::objects::Object_CreateAndCall_Create *>(msg->getContent());

				occ->func(_goFactory.createGO(occ->tpl, occ->getID(), occ->owner, occ->uuid, occ->tmpl, occ->send));
			} else {
				ISIXE_THROW_FAILURE("ObjectManager", "Invalid create message subtype: " << msg->getSubtype());
			}
			return;
		} else if (msg->getMessageType() == api::messages::ObjectManagerMessageType) {
			if (type == api::objects::ObjLevel) {
				std::string file = static_cast<api::objects::Object_Level_Create *>(msg->getContent())->file;
				std::string flags = static_cast<api::objects::Object_Level_Create *>(msg->getContent())->flags;

				loadLevel(file, flags);

				auto func = static_cast<api::objects::Object_Level_Create *>(msg->getContent())->func;
				if (func != nullptr) {
					func();
				}
			} else if (type == api::objects::ObjRegisterCTemplate) {
				std::string name = static_cast<api::objects::Object_RegisterCTemplate_Create *>(msg->getContent())->name;
				api::createGOCCallback func = static_cast<api::objects::Object_RegisterCTemplate_Create *>(msg->getContent())->func;

				registerCTemplate(name, func);
			}
		} else {
			ISIXE_THROW_FAILURE("ObjectManager", "Invalid create message type: " << msg->getMessageType());
		}
	}

	void ObjectManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (msg->getMessageType() == api::messages::ObjectManagerMessageType) {
			if (msg->getSubtype() == api::objects::ObjState) {
				core::IPKey receiver = dynamic_cast<api::objects::Object_State_Update *>(msg->getContent())->receiver;

				sendObjectMessages(receiver);
			} else if (msg->getSubtype() == api::objects::ObjConditionalMessage) {
				sendConditionalMessage(*static_cast<api::objects::Object_ConditionalMessage_Update *>(msg->getContent()));
			} else if (msg->getSubtype() == api::objects::ObjPause) {
				_paused = dynamic_cast<api::objects::Object_Pause_Update *>(msg->getContent())->pause;
			} else {
				ISIXE_THROW_FAILURE("ObjectManager", "Invalid update message type: " << msg->getMessageType());
			}
		} else {
			ISIXE_THROW_FAILURE("ObjectManager", "Invalid update message type: " << msg->getMessageType());
		}
	}

	void ObjectManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (msg->getMessageType() == api::messages::ObjectMessageType) {
			// Message to delete a GameObject
			if (type == api::objects::ObjCreate) {
				deleteObject(msg->getContent()->getID());
			} else {
				ISIXE_THROW_FAILURE("ObjectManager", "Invalid delete message type: " << msg->getSubtype());
			}
		} else if (msg->getMessageType() == api::messages::ObjectManagerMessageType) {
			// Messages for the ObjectManager
			if (type == api::objects::ObjType) {
				std::string objtype = static_cast<api::objects::Object_Type_Delete *>(msg->getContent())->type;

				deleteAllObjectsOfType(objtype);
			} else if (type == api::objects::ObjClean) {
				_GOList.clear();
				api::EngineController::GetSingletonPtr()->getObjectFacade()->updateGOList(_GOList);
				_tickList.clear();
				GOPtr::clear();
				api::ComPtr::clear();
			} else {
				ISIXE_THROW_FAILURE("ObjectManager", "Invalid delete message type: " << msg->getSubtype());
			}
		} else {
			ISIXE_THROW_FAILURE("ObjectManager", "Invalid delete message type: " << msg->getMessageType());
		}
	}

	void ObjectManager::tickComponents() {
		ASSERT_THREAD_SAFETY_FUNCTION
		std::vector<int64_t> removed;
		for (std::pair<int64_t, api::WeakComPtr> & p : _tickList) {
			api::ComPtr c = p.second.get();
			if (c == nullptr) {
				removed.push_back(p.first);
				continue;
			}
			c->Tick();
		}
		for (int64_t i : removed) {
			_tickList.erase(i);
		}
	}

	void ObjectManager::addTicker(const api::WeakComPtr & c) { // No lock necessary because Components only can be added during Object Tick
		ASSERT_THREAD_SAFETY_FUNCTION
		_tickList[c.get()->getID()] = c;
	}

	void ObjectManager::removeTicker(int64_t c) { // No lock necessary because Components only can be removed during Object Tick
		ASSERT_THREAD_SAFETY_FUNCTION
		_tickList.erase(c);
	}

	void ObjectManager::sendObjectMessages(const core::IPKey & receiver) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		std::vector<api::GameMessage::Ptr> v;

		for (std::list<GOPtr>::const_iterator it = _GOList.begin(); it != _GOList.end(); ++it) {
			assert(*it != nullptr);

			(*it)->synchronize(v, false);
		}

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::AdministrationMessageType, api::network::NetGameState, core::Method::Create, new api::network::Administration_GameState_Create(receiver, v), core::Subsystem::Object);

		api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
	}

	void ObjectManager::deleteObject(int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION

		for (std::list<GOPtr>::iterator iter = _GOList.begin(); iter != _GOList.end(); ++iter) {
			if ((*iter)->getID() == id) {
				_GOList.erase(iter);
				break;
			}
		}
		api::EngineController::GetSingletonPtr()->getObjectFacade()->updateGOList(_GOList);
		GOPtr::clear();
		api::ComPtr::clear();
	}

	void ObjectManager::insertObject(const GOPtr & go) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_GOList.push_back(go);
		api::EngineController::GetSingletonPtr()->getObjectFacade()->updateGOList(_GOList);
	}

	void ObjectManager::deleteAllObjectsOfType(const std::string & type) {
		ASSERT_THREAD_SAFETY_FUNCTION

		for (std::list<GOPtr>::iterator iter = _GOList.begin(); iter != _GOList.end(); ) {
			if ((*iter)->getType() == type) {
				std::list<GOPtr>::iterator iterBack = iter;
				iter++;
				_GOList.erase(iterBack);
			} else {
				iter++;
			}
		}
		api::EngineController::GetSingletonPtr()->getObjectFacade()->updateGOList(_GOList);
		GOPtr::clear();
		api::ComPtr::clear();
	}

	GOPtr ObjectManager::getObject(const int64_t goid) const {
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

	void ObjectManager::sendConditionalMessage(const api::objects::Object_ConditionalMessage_Update & m) {
		for (std::list<GOPtr>::iterator it = _GOList.begin(); it != _GOList.end(); ++it) {
			if (m.func(*it)) {
				api::GameMessage::Ptr msg = m.msg;
				if (m.sync || m.comFamID != UINT32_MAX) {
					msg = boost::make_shared<api::GameMessage>(*msg);
				}

				if (m.comFamID != UINT32_MAX) {
					api::ComPtr com = (*it)->getGOC(m.comFamID);
					if (com != nullptr) {
						msg->getContent()->_id = com->getID();
						msg->getContent()->_waitForId = (*it)->getID();
					} else {
						continue;
					}
				}

				if (m.sync) {
					msg->getContent()->_waitForId = (*it)->getID();
					api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
				}
				(*it)->News(msg);
			}
		}
	}

} /* namespace modules */
} /* namespace i6engine */
