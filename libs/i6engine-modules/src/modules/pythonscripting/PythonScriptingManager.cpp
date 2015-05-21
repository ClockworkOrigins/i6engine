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

#include "i6engine/modules/pythonscripting/PythonScriptingManager.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ScriptingConfig.h"
#include "i6engine/api/facades/ScriptingFacade.h"

namespace i6engine {
namespace modules {

	PythonScriptingManager::PythonScriptingManager() : _scripts(), _scriptsPath(), _callScripts() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "PythonScriptsPath", _scriptsPath)) {
			ISIXE_THROW_FAILURE("PythonScriptingController", "An exception has occurred: value PythonScriptsPath in section SCRIPT not found!");
		}
#if ISIXE_SCRIPTING == SCRIPTING_PYTHON
		api::EngineController::GetSingleton().getScriptingFacade()->_manager = this;
#endif
	}

	PythonScriptingManager::~PythonScriptingManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_scripts.clear();
	}

	void PythonScriptingManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		while (!_callScripts.empty()) {
			_callScripts.poll()();
		}
	}

	void PythonScriptingManager::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::scripting::ScrRayResult) {
			std::string file = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->func;

			callScript<void>(file, func, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->raytestResult, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->rayID);
		} else if (type == api::scripting::ScrLoadAllScripts) {
			ISIXE_THROW_FAILURE("PythonScriptingManager", "Loading of all scripts isn't implemented yet in PythonScriptingManager!");
		} else {
			ISIXE_THROW_MESSAGE("PythonScriptingManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void PythonScriptingManager::parseScript(const std::string & file) {
		ASSERT_THREAD_SAFETY_FUNCTION
		try {
			if (_scripts.find(file) == _scripts.end()) {
				boost::python::object module = boost::python::import(utils::split(file, "/").back().c_str());
				boost::python::object global = module.attr("__dict__");
				boost::python::exec_file((_scriptsPath + "/" + file + ".py").c_str(), global, global);

				_scripts[file] = global;
			}
		} catch(const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

} /* namespace modules */
} /* namespace i6engine */