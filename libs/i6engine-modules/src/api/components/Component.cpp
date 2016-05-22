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

#include "i6engine/api/components/Component.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	Component::Component() : _objOwnerID(), _objOwnerGO(), _objComponentID(), _objFamilyID(), _id(-1), _sync(true), _self(), _tickingAllowed(true), _wantsToTick(false), _isTicking(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_id = EngineController::GetSingletonPtr()->getIDManager()->getID();
	}

	Component::Component(const int64_t id, const attributeMap & params) : _objOwnerID(), _objOwnerGO(), _objComponentID(), _objFamilyID(), _id(id), _sync(true), _self(), _tickingAllowed(true), _wantsToTick(false), _isTicking(false) {
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
		if (msg->getSubtype() == components::ComponentMessageTypes::ComEnableTicking) {
			doEnableTicking(dynamic_cast<components::Component_EnableTicking_Update *>(msg->getContent())->allowTicking);
		} else {
			ISIXE_THROW_FAILURE("Component", "News called without News function overridden: " << msg->getMessageInfo() << " " << getComponentID());
		}
	}

	void Component::enableTicking(bool allowTicking) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComEnableTicking, core::Method::Update, new components::Component_EnableTicking_Update(_objOwnerID, getID(), allowTicking), core::Subsystem::Unknown));
	}

	std::pair<AddStrategy, int64_t> Component::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::REPLACE_DIS, 0);
	}

	void Component::addTicker() {
		if (_tickingAllowed) {
			EngineController::GetSingletonPtr()->getObjectFacade()->addTicker(_self);
			_isTicking = true;
		} else {
			_wantsToTick = true;
		}
	}

	void Component::removeTicker() {
		if (_tickingAllowed) {
			EngineController::GetSingletonPtr()->getObjectFacade()->removeTicker(_id);
			_isTicking = false;
		}
		_wantsToTick = false;
	}

	void Component::doEnableTicking(bool allowTicking) {
		_tickingAllowed = allowTicking;
		if (_tickingAllowed) {
			if (_wantsToTick) {
				EngineController::GetSingletonPtr()->getObjectFacade()->addTicker(_self);
				_wantsToTick = false;
				_isTicking = true;
			}
		} else {
			if (_isTicking) {
				EngineController::GetSingletonPtr()->getObjectFacade()->removeTicker(_id);
				_wantsToTick = true;
			}
		}
	}

} /* namespace api */
} /* namespace i6e */
