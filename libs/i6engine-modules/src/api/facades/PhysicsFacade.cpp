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

#include "i6engine/api/facades/PhysicsFacade.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	void PhysicsFacade::setGravity(const Vec3 & vec3Gravity) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyGravity, core::Method::Update, new physics::Physics_Gravity_Update(vec3Gravity), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void PhysicsFacade::registerNotifyCallback(const boost::function<void(int64_t)> & f) {
		_notify = f;
	}

	void PhysicsFacade::notifyNewID(int64_t id) {
		if (!_notify.empty()) {
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
} /* namespace i6engine */
