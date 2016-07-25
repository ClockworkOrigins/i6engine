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

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/configs/ObjectConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"

namespace i6e {
namespace api {

	GameObject::GameObject(const int64_t goid, const core::IPKey & owner, uint64_t uuid, const std::string & tpl, const std::function<ComPtr(const int64_t, const std::string &, const attributeMap &, const WeakGOPtr &)> & f) : _type(), _objGoid(goid), _objComponents(), _template(tpl), _messages(), _owner(owner), _uuid(uuid), _componentFunc(f), _self(), _flags(), _send(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		if (_objGoid == -1) {
			_objGoid = EngineController::GetSingletonPtr()->getIDManager()->getID();
		}
	}

	GameObject::~GameObject() {
		ASSERT_THREAD_SAFETY_FUNCTION
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Delete, new graphics::Graphics_Node_Delete(_objGoid), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		clearGOCs();
	}

	ComPtr GameObject::getGOC(uint32_t famID) const { // TODO: (Daniel) thread safety
		component_table_t::const_iterator it = _objComponents.find(famID);
		// GOComponent not found
		if (it == _objComponents.end()) {
			return ComPtr();
		}

		// Return a pointer to the found GOComponent
		return it->second;
	}

	ComPtr GameObject::getGOC(uint32_t famID, const std::string & identifier) const {
		component_table_t::const_iterator it = _objComponents.find(famID);
		// GOComponent not found
		if (it == _objComponents.end()) {
			return ComPtr();
		}

		if (it->second->getIdentifier() != identifier) {
			for (ComPtr & c : it->second->_subComps) {
				if (c->getIdentifier() == identifier) {
					return c;
				}
			}

			return ComPtr();
		}

		// Return a pointer to the found GOComponent
		return it->second;
	}

	ComPtr GameObject::getGOCID(const int64_t id) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		// GOComponent not found
		for (const component_table_pair_t & com : _objComponents) {
			if (com.second->getID() == id) {
				return com.second;
			}

			for (ComPtr & c : com.second->_subComps) {
				if (c->getID() == id) {
					return c;
				}
			}
		}
		return ComPtr();
	}

	std::vector<ComPtr> GameObject::getGOCList() const {
		std::vector<ComPtr> result;

		for (auto & c : _objComponents) {
			result.push_back(c.second);

			for (auto & sc : c.second->_subComps) {
				result.push_back(sc);
			}
		}

		return result;
	}

	std::pair<bool, int64_t> GameObject::setGOC(const ComPtr & objNewGOC) {
		ASSERT_THREAD_SAFETY_FUNCTION
		// Check, if the new GOC is nullptr
		if (objNewGOC == nullptr) {
			ISIXE_THROW_FAILURE("OBJECT", "The following component was nullptr: " << objNewGOC->getComponentID());
		}

		// Get family id
		uint32_t famID = objNewGOC->getFamilyID();

		// Check, if GameObject already has a GOComponent with the family id of objNewGOC
		ComPtr objOldGOC = getGOC(famID);
		bool ret = true;
		int64_t id = -1;
		if (objOldGOC != nullptr) {
			std::pair<AddStrategy, int64_t> add = objOldGOC->howToAdd(objNewGOC);
			switch (add.first) {
			case AddStrategy::ADD: {
				objOldGOC->_subComps.push_back(objNewGOC); // append it
				break;
			}
			case AddStrategy::REPLACE: {
				if (add.second == -1) { // replace the dispatcher
					_objComponents[famID]->Finalize();
					_objComponents[famID] = objNewGOC; // set new dispatcher
					objNewGOC->_subComps = objOldGOC->_subComps;
					objOldGOC->_subComps.clear();
				} else { // replace another component
					objOldGOC->_subComps[size_t(add.second)]->Finalize();
					objOldGOC->_subComps[size_t(add.second)] = objNewGOC;
				}
				id = objOldGOC->getID();
				break;
			}
			case AddStrategy::REPLACE_DIS: {
				_objComponents[famID]->Finalize();
				_objComponents[famID] = objNewGOC; // add new component
				id = objOldGOC->getID();
				break;
			}
			case AddStrategy::REJECT: {
				ret = false;
				break;
			}
			default: {
				ISIXE_THROW_FAILURE("GameObject", "unknown AddStrategy");
			}
			}
			if (add.first != AddStrategy::REJECT) {
				objNewGOC->setOwnerGO(_self);
			}
		} else {
			// Insert new GOComponent into component table of GameObject
			_objComponents[famID] = objNewGOC;
			objNewGOC->setOwnerGO(_self);
		}
		return std::make_pair(ret, id);
	}

	void GameObject::clearGOCs() {
		for (auto & component : _objComponents) {
			for (auto & subComp : component.second->_subComps) {
				subComp->Finalize();
			}
			component.second->Finalize();
			component.second->_subComps.clear();
		}
		// Clear the map to avoid memory leaks
		_objComponents.clear();
	}

	void GameObject::setType(const std::string & type) {
		_type = type;
	}

	void GameObject::sendCreateMessage() const {
		ASSERT_THREAD_SAFETY_FUNCTION
		std::vector<GameMessage::Ptr> v;
		synchronize(v, false);

		EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, v[0]);
	}

	void GameObject::deleteGOC(uint32_t famID) {
		ASSERT_THREAD_SAFETY_FUNCTION
		// If the map is empty, there is nothing to do
		component_table_t::const_iterator it = _objComponents.find(famID);

		// GOComponent not found
		if (it == _objComponents.end()) {
			ISIXE_THROW_FAILURE("GameObject", "Couldn't find the following component: " << famID);
		} else {
			deleteGOC(famID, it->second->getIdentifier());
		}
	}

	void GameObject::deleteGOC(uint32_t famID, const std::string & identifier) {
		ASSERT_THREAD_SAFETY_FUNCTION
		// TODO: (Daniel) implement
		component_table_t::const_iterator it = _objComponents.find(famID);
		if (it == _objComponents.end()) {
			ISIXE_THROW_FAILURE("GameObject", "Couldn't find the following component: " << famID);
		} else {
			if (it->second->getIdentifier() == identifier) { // delete dispatcher
				it->second->Finalize();
				ComPtr old = _objComponents[famID];
				if (old->_subComps.empty()) { // deletes the last Component with this famID
					_objComponents.erase(it);
				} else {
					_objComponents[famID] = old->_subComps[0];
					_objComponents[famID]->_subComps = std::vector<ComPtr>(old->_subComps.begin() + 1, old->_subComps.end());
					old->_subComps.clear();
				}
			} else {
				std::vector<ComPtr>::iterator it2 = std::find_if(it->second->_subComps.begin(), it->second->_subComps.end(),
					[identifier](const ComPtr & cc) {
						return cc->getIdentifier() == identifier;
					});
				if (it2 == it->second->_subComps.end()) {
					ISIXE_THROW_FAILURE("GameObject", "Couldn't find component: " << famID << ":" << identifier);
				} else {
					(*it2)->Finalize();
					it->second->_subComps.erase(it2);
				}
			}
		}
	}

	void GameObject::setDie() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ObjectMessageType, objects::ObjCreate, core::Method::Delete, new objects::Object_Create_Delete(getID(), getOwner()), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
	}

	void GameObject::News(const GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getMessageType() == messages::ComponentMessageType) {
			if (msg->getMethod() == core::Method::Create) {
				if (msg->getSubtype() == components::ComCreate) {
					// Create new Component
					int64_t compID = static_cast<components::Component_Create_Create *>(msg->getContent())->_id;
					std::string templ = static_cast<components::Component_Create_Create *>(msg->getContent())->tpl;
					attributeMap params = static_cast<components::Component_Create_Create *>(msg->getContent())->params;

					ComPtr c = _componentFunc(compID, templ, params, _self);
					if (c != nullptr) {
						c->Init();
					}
				} else if (msg->getSubtype() == components::ComCreateCallback) {
					// Create new Component
					int64_t compID = static_cast<components::Component_CreateCallback_Create *>(msg->getContent())->_id;
					std::string templ = static_cast<components::Component_CreateCallback_Create *>(msg->getContent())->tpl;
					attributeMap params = static_cast<components::Component_CreateCallback_Create *>(msg->getContent())->params;
					std::function<void(ComPtr)> callback = static_cast<components::Component_CreateCallback_Create *>(msg->getContent())->callback;

					ComPtr c = _componentFunc(compID, templ, params, _self);
					if (c != nullptr) {
						c->Init();
					}
					callback(c);
				}
			} else if (msg->getMethod() == core::Method::Update) {
				// forward to component
				int64_t comID = msg->getContent()->_id;
				ComPtr com = getGOCID(comID);
				if (com == nullptr) { // (Daniel) Why can it happen that the message is delivered but the Component doesn't exist? Component deletion is possible in two scenarios: GameObject is deleted (can't arrive here) or setDie is called which must block all further messages for the comID
					ISIXE_LOG_ERROR("GameObject", "News: Component with id " << comID << " not part of this GameObject: " << msg->getMessageInfo() << " for GameObject of type: " << getType()); // TODO: (Daniel) check where the component really is deleted and fix it, this is only a workaround!!!
					return;
				}
				com->News(msg);
			} else if (msg->getMethod() == core::Method::Delete) {
				// TODO: (Michael) don't use this indirection
				int64_t comID = msg->getContent()->_id;
				ComPtr com = getGOCID(comID);

				if (com == nullptr) {
					ISIXE_LOG_ERROR("GameObject", "News: Component not part of this GameObject: " << msg->getMessageInfo() << " for GameObject of type: " << getType()); // TODO: (Daniel) check where the component really is deleted and fix it, this is only a workaround!!!
					return;
				}
				deleteGOC(com->getFamilyID(), com->getIdentifier());
			}
		} else if (msg->getMessageType() == messages::ObjectMessageType) {
			if (msg->getMethod() != core::Method::Update) {
				ISIXE_THROW_FAILURE("GameObject", "News: Only Update Messages should occure here: " << msg->getMessageInfo());
				return;
			}

			if (msg->getSubtype() == objects::ObjKillMe) {
				setDie();
			}
		} else if (msg->getMessageType() == messages::PositionMessageType) {
			if (msg->getMethod() != core::Method::Update) {
				ISIXE_THROW_FAILURE("GameObject", "News: Only Update Messages should occure here: " << msg->getMessageInfo());
				return;
			}

			int64_t comID = msg->getContent()->_id;
			getGOCID(comID)->News(msg);
		} else {
			ISIXE_THROW_FAILURE("GameObject", "News: Unknown Type for GameObject messages: " << msg->getMessageInfo());
		}
	}

	void GameObject::synchronize(std::vector<GameMessage::Ptr> & messages, bool all) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		objects::GOTemplate tpl;
		tpl._type = getType();
		for (const std::pair<int64_t, ComPtr> & p : _objComponents) {
			if (p.second->getFamilyID() != components::NetworkSenderComponent && (all || p.second->getSync())) {
				tpl._components.push_back(objects::GOTemplateComponent(p.second->getTemplateName(), p.second->getID(), p.second->synchronize(), false, p.second->getIdentifier(), false));
			}
			for (ComPtr & c : p.second->_subComps) {
				if (c->getSync()) {
					tpl._components.push_back(objects::GOTemplateComponent(c->getTemplateName(), c->getID(), c->synchronize(), false, c->getIdentifier(), false));
				}
			}
		}

		messages.push_back(boost::make_shared<GameMessage>(messages::ObjectMessageType, objects::ObjCreate, core::Method::Create, new objects::Object_Create_Create(_objGoid, _template, _owner, _uuid, tpl, false), core::Subsystem::Unknown));
	}

	void GameObject::initializeComponents() {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (component_table_pair_t & p : _objComponents) {
			p.second->Init();

			for (ComPtr & c : p.second->_subComps) {
				c->Init();
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
