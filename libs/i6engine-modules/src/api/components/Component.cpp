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

#include "i6engine/api/components/Component.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	Component::Component() : _objOwnerID(), _objOwnerGO(), _objComponentID(), _objFamilyID(), _id(-1), _sync(true), _self() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_id = EngineController::GetSingletonPtr()->getIDManager()->getID();
	}

	Component::Component(const int64_t id, const attributeMap & params) : _objOwnerID(), _objOwnerGO(), _objComponentID(), _objFamilyID(), _id(id), _sync(true), _self() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		if (params.find("identifier") != params.end()) {
			_identifier = params.find("identifier")->second;
		}
	}

	Component::~Component() {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	void Component::setOwnerGO(const WeakGOPtr & go) {
		ASSERT_THREAD_SAFETY_FUNCTION
		GOPtr gop = go.get();

		if (gop != nullptr) {
			_objOwnerID = gop->getID();
			_objOwnerGO = go;
		}
	}

	void Component::setDie() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComCreate, core::Method::Delete, new components::Component_Create_Delete(_objOwnerID, getID(), getFamilyID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	GOPtr Component::getOwnerGO() {
		if (_objOwnerGO == nullptr) {
			_objOwnerGO = EngineController::GetSingletonPtr()->getObjectFacade()->getObject(_objOwnerID);
			assert(_objOwnerGO != nullptr);
		}

		return _objOwnerGO.get();
	}

	void Component::setSelf(const WeakComPtr & self) {
		_self = self;
	}

	void Component::News(const GameMessage::Ptr & msg) {
		ISIXE_THROW_FAILURE("Component", "News called without News function overridden: " << msg->getMessageInfo() + " " << getComponentID());
	}

	std::pair<AddStrategy, int64_t> Component::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::REPLACE_DIS, 0);
	}

	void Component::addTicker() {
		EngineController::GetSingletonPtr()->getObjectFacade()->addTicker(_self);
	}

	void Component::removeTicker() {
		EngineController::GetSingletonPtr()->getObjectFacade()->removeTicker(_id);
	}

} /* namespace api */
} /* namespace i6engine */
