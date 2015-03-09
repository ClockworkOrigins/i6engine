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

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/object/ObjectMailbox.h"
#include "i6engine/modules/object/ObjectManager.h"

namespace i6engine {
namespace modules {

	ObjectController::ObjectController() : core::ModuleController(core::Subsystem::Object), _mailbox(), _manager() {
	}

	ObjectController::~ObjectController() {
	}

	void ObjectController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		api::EngineController::GetSingletonPtr()->getObjectFacade()->registerNotifyCallback(boost::bind(&MessageSubscriber::notifyNewID, this, _1));
		ISIXE_REGISTERMESSAGETYPE(api::messages::ComponentMessageType, ObjectController::MailboxComponent);
		ISIXE_REGISTERMESSAGETYPE(api::messages::ObjectMessageType, ObjectController::MailboxObject);
		ISIXE_REGISTERMESSAGETYPE(api::messages::ObjectManagerMessageType, ObjectController::MailboxObjectManager);
		ISIXE_REGISTERMESSAGETYPE(api::messages::PositionMessageType, ObjectController::MailboxObject);

		_manager = new ObjectManager(this);
		_mailbox = new ObjectMailbox(_manager);

		api::EngineController::GetSingletonPtr()->getObjectFacade()->setFrametime(getFrameTime());
	}

	void ObjectController::MailboxComponent(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mailbox->NewsComponent(msg);
	}

	void ObjectController::MailboxObject(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mailbox->NewsObject(msg);
	}

	void ObjectController::MailboxObjectManager(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mailbox->NewsObjectManager(msg);
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
} /* namespace i6engine */
