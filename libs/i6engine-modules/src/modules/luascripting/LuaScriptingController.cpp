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

#include "i6engine/modules/luascripting/LuaScriptingController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/luascripting/LuaScriptingMailbox.h"
#include "i6engine/modules/luascripting/LuaScriptingManager.h"

namespace i6engine {
namespace modules {

	LuaScriptingController::LuaScriptingController() : core::ModuleController(core::Subsystem::Scripting), _mailbox(), _manager() {
	}

	LuaScriptingController::~LuaScriptingController() {
	}

	void LuaScriptingController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_manager = new LuaScriptingManager();
		_mailbox = new LuaScriptingMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::ScriptingMessageType, LuaScriptingMailbox::News, _mailbox);
	}

	void LuaScriptingController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_LOG_INFO("LuaScriptingController", "***Shutting down subsystem***");

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ScriptingMessageType);
		delete _manager;
		delete _mailbox;
	}

	void LuaScriptingController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

} /* namespace modules */
} /* namespace i6engine */
