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

#include "i6engine/modules/gui/GUIController.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"

#include "i6engine/modules/gui/GUIMailbox.h"
#include "i6engine/modules/gui/GUIManager.h"

namespace i6e {
namespace modules {

	GUIController::GUIController() : core::ModuleController(core::Subsystem::GUI), _manager(nullptr), _mailbox(nullptr) {
	}

	GUIController::~GUIController() {
	}

	void GUIController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		_manager = new GUIManager(this);
		_mailbox = new GUIMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::GUIMessageType, GUIMailbox::News, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, GUIMailbox::News, _mailbox);
	}

	void GUIController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		try {
			core::MessageSubscriber::processMessages();

			_manager->Tick();
		} catch(utils::exceptions::SubsystemException & e) {
			e.writeLog();
			// check errorcode and try to repair, if not possible:
			e.PassToMain();
		}
	}

	void GUIController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::GUIMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		ISIXE_LOG_INFO("GuiController", "*** Shutting Down Subsystem ***");

		delete _mailbox;
		_mailbox = nullptr;
		delete _manager;
		_manager = nullptr;
	}

} /* namespace modules */
} /* namespace i6e */
