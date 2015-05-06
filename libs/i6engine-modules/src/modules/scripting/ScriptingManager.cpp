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

#include "i6engine/modules/scripting/ScriptingManager.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ScriptingConfig.h"

namespace i6engine {
namespace modules {

	ScriptingManager::ScriptingManager() : _scripts(), _scriptsPath() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "PythonScriptsPath", _scriptsPath)) {
			ISIXE_THROW_FAILURE("ScriptingController", "An exception has occurred: value PythonScriptsPath in section SCRIPT not found!");
			return;
		}
	}

	ScriptingManager::~ScriptingManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_scripts.clear();
	}

	void ScriptingManager::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::scripting::ScrCall) {
			std::string file = static_cast<api::scripting::Scripting_Call_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_Call_Update *>(msg->getContent())->func;

			callScript<void>(file, func);
		} else if (type == api::scripting::ScrCallID) {
			std::string file = static_cast<api::scripting::Scripting_CallID_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_CallID_Update *>(msg->getContent())->func;

			callScript<void>(file, func, static_cast<api::scripting::Scripting_CallID_Update *>(msg->getContent())->intParam);
		} else if (type == api::scripting::ScrCallID2) {
			std::string file = static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->func;

			callScript<void>(file, func, static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->intParam, static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->intParam2);
		} else if (type == api::scripting::ScrCallID2Double) {
			std::string file = static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->func;

			callScript<void>(file, func, static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->intParam, static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->doubleParam);
		} else if (type == api::scripting::ScrRayResult) {
			std::string file = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->func;

			callScript<void>(file, func, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->raytestResult, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->rayID);
		} else {
			ISIXE_THROW_MESSAGE("ScriptingManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void ScriptingManager::parseScript(const std::string & file) {
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
