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

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ScriptingConfig.h"

#include "i6engine/modules/scripting/ScriptingController.h"

#include "boost/python.hpp"

namespace i6engine {
namespace modules {

	ScriptingManager::ScriptingManager(ScriptingController * ctrl) : _scripts(), _ctrl(ctrl) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	ScriptingManager::~ScriptingManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_scripts.clear();
	}

	void ScriptingManager::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		int type = msg->getSubtype();

		if (type == api::scripting::ScrCall) {
			std::string file = static_cast<api::scripting::Scripting_Call_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_Call_Update *>(msg->getContent())->func;

			callScript(file, func);
		} else if (type == api::scripting::ScrCallID) {
			std::string file = static_cast<api::scripting::Scripting_CallID_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_CallID_Update *>(msg->getContent())->func;

			callScript(file, func, static_cast<api::scripting::Scripting_CallID_Update *>(msg->getContent())->intParam);
		} else if (type == api::scripting::ScrCallID2) {
			std::string file = static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->func;

			callScript(file, func, static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->intParam, static_cast<api::scripting::Scripting_CallID2_Update *>(msg->getContent())->intParam2);
		} else if (type == api::scripting::ScrCallID2Double) {
			std::string file = static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->func;

			callScript(file, func, static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->intParam, static_cast<api::scripting::Scripting_CallID2Double_Update *>(msg->getContent())->doubleParam);
		} else if (type == api::scripting::ScrRayResult) {
			std::string file = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->func;

			callScript(file, func, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->raytestResult, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->rayID);
		} else if (msg->getSubtype() == api::scripting::ScrReset) {
			_ctrl->reset();
		} else {
			//ISIXE_THROW_MESSAGE("ScriptingManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void ScriptingManager::callScript(const std::string & file, const std::string & func) {
		ASSERT_THREAD_SAFETY_FUNCTION
		parseScript(file, func);

		try {
			boost::python::object f = _scripts[file][func];

			f();
		} catch(const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

	void ScriptingManager::callScript(const std::string & file, const std::string & func, const int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION
		parseScript(file, func);

		try {
			boost::python::object f = _scripts[file][func];

			f(id);
		} catch(const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

	void ScriptingManager::callScript(const std::string & file, const std::string & func, const int64_t id, const int64_t id2) {
		ASSERT_THREAD_SAFETY_FUNCTION
		parseScript(file, func);

		try {
			boost::python::object f = _scripts[file][func];

			f(id, id2);
		} catch (const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

	void ScriptingManager::callScript(const std::string & file, const std::string & func, const int64_t id, const double d) {
		ASSERT_THREAD_SAFETY_FUNCTION
		parseScript(file, func);

		try {
			boost::python::object f = _scripts[file][func];

			f(id, d);
		} catch (const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

	void ScriptingManager::callScript(const std::string & file, const std::string & func, const api::RayTestResult & rtr, const int64_t rayID) {
		ASSERT_THREAD_SAFETY_FUNCTION
		parseScript(file, func);

		try {
			boost::python::object f = _scripts[file][func];

			f(rtr, rayID);
		} catch(const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

	void ScriptingManager::parseScript(const std::string & file, const std::string & func) {
		ASSERT_THREAD_SAFETY_FUNCTION
		try {
			if (_scripts.find(file) == _scripts.end()) {
				boost::python::object module = boost::python::import(utils::split(utils::split(file, "/").back(), ".").front().c_str());
				boost::python::object global = module.attr("__dict__");
				boost::python::exec_file(file.c_str(), global, global);

				_scripts[file] = global;
			}
		} catch(const boost::python::error_already_set &) {
			PyErr_PrintEx(0);
		}
	}

} /* namespace modules */
} /* namespace i6engine */
