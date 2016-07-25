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

#include "i6engine/api/facades/PhysicsFacade.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace api {

	void PhysicsFacade::setGravity(const Vec3 & vec3Gravity) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyGravity, core::Method::Update, new physics::Physics_Gravity_Update(vec3Gravity), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void PhysicsFacade::registerNotifyCallback(const std::function<void(int64_t)> & f) {
		_notify = f;
	}

	void PhysicsFacade::notifyNewID(int64_t id) {
		if (_notify != nullptr) {
			_notify(id);
		}
	}

	void PhysicsFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void PhysicsFacade::pause() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyPause, core::Method::Update, new physics::Physics_Pause_Update(true), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void PhysicsFacade::unpause() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyPause, core::Method::Update, new physics::Physics_Pause_Update(false), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace api */
} /* namespace i6e */
