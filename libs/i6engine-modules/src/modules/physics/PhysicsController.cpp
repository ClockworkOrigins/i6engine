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

#include "i6engine/modules/physics/PhysicsController.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"

#include "i6engine/modules/physics/PhysicsMailbox.h"
#include "i6engine/modules/physics/PhysicsManager.h"

namespace i6e {
namespace modules {

	PhysicsController::PhysicsController() : core::ModuleController(core::Subsystem::Physic), _manager(), _mailbox() {}

	PhysicsController::~PhysicsController() {
	}

	void PhysicsController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		api::EngineController::GetSingletonPtr()->getPhysicsFacade()->registerNotifyCallback(boost::bind(&MessageSubscriber::notifyNewID, this, _1));

		_manager = new PhysicsManager(this);
		_mailbox = new PhysicsMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::PhysicsMessageType, PhysicsMailbox::News, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::PhysicsNodeMessageType, PhysicsMailbox::News, _mailbox);
	}

	void PhysicsController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_LOG_INFO("PhysicsController", "*** Shutting Down Subsystem ***");

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::PhysicsMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::PhysicsNodeMessageType);

		delete _mailbox;
		_mailbox = nullptr;
		delete _manager;
		_manager = nullptr;
	}

	void PhysicsController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

} /* namespace modules */
} /* namespace i6e */
