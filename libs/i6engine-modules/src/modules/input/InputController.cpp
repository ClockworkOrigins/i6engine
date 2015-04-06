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

#include "i6engine/modules/input/InputController.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/input/InputMailbox.h"
#include "i6engine/modules/input/InputManager.h"

namespace i6engine {
namespace modules {

	InputController::InputController() : core::ModuleController(core::Subsystem::Input), _mailbox(nullptr), _manager(nullptr) {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, InputController::Mailbox, this);
	}

	void InputController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_manager = new InputManager(this);
		_mailbox = new InputMailbox(_manager);
	}

	void InputController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION

		ISIXE_LOG_INFO("InputController", "***Shutting down subsystem***");
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);

		delete _mailbox;
		_mailbox = nullptr;
		delete _manager;
		_manager = nullptr;
	}

	void InputController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

	void InputController::Mailbox(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mailbox->News(msg);
	}

} /* namespace modules */
} /* namespace i6engine */
