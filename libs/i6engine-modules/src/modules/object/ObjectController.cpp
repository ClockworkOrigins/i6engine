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

#include "i6engine/modules/object/ObjectController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/object/ObjectMailbox.h"
#include "i6engine/modules/object/ObjectManager.h"

namespace i6e {
namespace modules {

	ObjectController::ObjectController() : core::ModuleController(core::Subsystem::Object), _mailbox(), _manager() {
	}

	ObjectController::~ObjectController() {
	}

	void ObjectController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		api::EngineController::GetSingletonPtr()->getObjectFacade()->registerNotifyCallback(boost::bind(&MessageSubscriber::notifyNewID, this, _1));

		_manager = new ObjectManager();
		_mailbox = new ObjectMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::ComponentMessageType, ObjectMailbox::NewsComponent, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::ObjectMessageType, ObjectMailbox::NewsObject, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::ObjectManagerMessageType, ObjectMailbox::NewsObjectManager, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::PositionMessageType, ObjectMailbox::NewsObject, _mailbox);

		api::EngineController::GetSingletonPtr()->getObjectFacade()->setFrametime(getFrameTime());
	}

	void ObjectController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ComponentMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ObjectMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ObjectManagerMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::PositionMessageType);

		ISIXE_LOG_INFO("ObjectController", "*** Shutting Down Subsystem ***");

		delete _mailbox;
		delete _manager;

		_mailbox = nullptr;
		_manager = nullptr;
	}

	void ObjectController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

} /* namespace modules */
} /* namespace i6e */
