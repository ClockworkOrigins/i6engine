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

#include "i6engine/modules/pythonscripting/PythonScriptingController.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/pythonscripting/PythonScriptingMailbox.h"
#include "i6engine/modules/pythonscripting/PythonScriptingManager.h"

#include "boost/filesystem.hpp"
#include "boost/python.hpp"

namespace i6e {
namespace modules {

	PythonScriptingController::PythonScriptingController() : core::ModuleController(core::Subsystem::Scripting), _mailbox(), _manager() {
	}

	PythonScriptingController::~PythonScriptingController() {
	}

	void PythonScriptingController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_manager = new PythonScriptingManager();
		_mailbox = new PythonScriptingMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::ScriptingMessageType, PythonScriptingMailbox::News, _mailbox);
	}

	void PythonScriptingController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_LOG_INFO("PythonScriptingController", "***Shutting down subsystem***");

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ScriptingMessageType);
		delete _manager;
		delete _mailbox;
	}

	void PythonScriptingController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
		// *** Tick Code ***
		if (PyErr_CheckSignals() == -1) {
			ISIXE_LOG_DEBUG("PythonScriptingController", "CTRL-C");
			api::EngineController::GetSingletonPtr()->stop();
		}
	}

} /* namespace modules */
} /* namespace i6e */
