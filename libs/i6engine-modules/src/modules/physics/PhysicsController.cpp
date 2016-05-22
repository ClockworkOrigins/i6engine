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
