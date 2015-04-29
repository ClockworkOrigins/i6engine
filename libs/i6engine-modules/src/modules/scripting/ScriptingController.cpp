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

#include "i6engine/modules/scripting/ScriptingController.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/scripting/ScriptingMailbox.h"
#include "i6engine/modules/scripting/ScriptingManager.h"

#include "boost/filesystem.hpp"
#include "boost/python.hpp"

namespace i6engine {
namespace modules {

// Phyton API needs constant char *
namespace PythonAPIWorkaround {
	char path[] = "path";
}

	ScriptingController::ScriptingController() : core::ModuleController(core::Subsystem::Scripting), _mailbox(), _manager() {
	}

	ScriptingController::~ScriptingController() {
	}

	void ScriptingController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		// *** Initialization goes here ***
		Py_Initialize();
		ISIXE_LOG_INFO("ScriptingController", Py_GetVersion());

		boost::filesystem::path workingDir = boost::filesystem::complete("./").normalize();
		PyObject * sysPath = PySys_GetObject(PythonAPIWorkaround::path);
		PyList_Insert(sysPath, 0, PyString_FromString(workingDir.string().c_str()));
		std::string mainDir;
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue<std::string>("GENERAL", "i6engineMainDir", mainDir)) {
			ISIXE_LOG_WARN("Scripting", "No 'i6engineMainDir' path set in category 'GENERAL' in the config file. No additional path will be added");
		} else {
			mainDir += "/lib";
			PyList_Insert(sysPath, 0, PyString_FromString(mainDir.c_str()));
		}

		std::string scriptsPath;
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue<std::string>("SCRIPT", "ScriptsPath", scriptsPath)) {
			ISIXE_LOG_ERROR("ScriptingController", "An exception has occurred: value ScriptsPath in section SCRIPT not found!");
			return;
		}

		boost::filesystem::path workingDir2 = boost::filesystem::complete(scriptsPath.c_str()).normalize();
		PyList_Insert(sysPath, 0, PyString_FromString(workingDir2.string().c_str()));

		_manager = new ScriptingManager();
		_mailbox = new ScriptingMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::ScriptingMessageType, ScriptingMailbox::News, _mailbox);
	}

	void ScriptingController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_LOG_INFO("ScriptingController", "***Shutting down subsystem***");

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ScriptingMessageType);
		Py_Finalize();
		delete _manager;
		delete _mailbox;
	}

	void ScriptingController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		// *** Tick Code ***
		if (PyErr_CheckSignals() == -1) {
			ISIXE_LOG_DEBUG("ScriptingController", "CTRL-C");
			api::EngineController::GetSingletonPtr()->stop();
		}
	}

} /* namespace modules */
} /* namespace i6engine */
