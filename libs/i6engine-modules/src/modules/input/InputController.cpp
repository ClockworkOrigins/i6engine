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

#include "i6engine/modules/input/InputController.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/input/InputMailbox.h"
#include "i6engine/modules/input/InputManager.h"

namespace i6e {
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
} /* namespace i6e */
