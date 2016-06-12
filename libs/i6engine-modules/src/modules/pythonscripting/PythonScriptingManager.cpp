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

#include "i6engine/modules/pythonscripting/PythonScriptingManager.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ScriptingConfig.h"
#include "i6engine/api/facades/ScriptingFacade.h"

#include "boost/filesystem.hpp"

namespace i6e {
namespace modules {

	PythonScriptingManager::PythonScriptingManager() : _scripts(), _scriptsPath(), _callScripts() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		// *** Initialization goes here ***
		Py_Initialize();
		ISIXE_LOG_INFO("PythonScriptingManager", Py_GetVersion());

		boost::filesystem::path workingDir = boost::filesystem::complete("./").normalize();
		PyObject * sysPath = PySys_GetObject("path");
		PyList_Insert(sysPath, 0, PyString_FromString(workingDir.string().c_str()));
		std::string mainDir;
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("GENERAL", "i6engineMainDir", mainDir)) {
			ISIXE_LOG_WARN("PythonScriptingManager", "No 'i6engineMainDir' path set in category 'GENERAL' in the config file. No additional path will be added");
		} else {
			mainDir += "/lib";
			PyList_Insert(sysPath, 0, PyString_FromString(mainDir.c_str()));
		}

		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "PythonScriptsPath", _scriptsPath)) {
			ISIXE_THROW_FAILURE("PythonScriptingManager", "An exception has occurred: value PythonScriptsPath in section SCRIPT not found!");
		}
		boost::filesystem::path workingDir2 = boost::filesystem::complete((_scriptsPath + "/").c_str()).normalize();
		PyList_Insert(sysPath, 0, PyString_FromString(workingDir2.string().c_str()));
#if ISIXE_SCRIPTING == SCRIPTING_PYTHON
		api::EngineController::GetSingleton().getScriptingFacade()->_manager = this;
#endif
		// preload all exports
		boost::python::object module = boost::python::import("__main__");
		boost::python::object global = module.attr("__dict__");

		boost::filesystem::directory_iterator iter("."), dirEnd;
		while (iter != dirEnd) {
			if (boost::filesystem::is_regular_file(*iter)) {
				std::string file = iter->path().string();
				if (file.find("Scripting") != std::string::npos && file.find("Python") != std::string::npos) {
					size_t n = file.find("Scripting");
					size_t n2 = file.find("Python");
					file = file.substr(n, n2 - n + 6);
					boost::python::exec(boost::python::str((std::string("import ") + file).c_str()), global, global);
					boost::python::exec(boost::python::str((std::string("from ") + file + std::string(" import *")).c_str()), global, global);
				}
			}
			iter++;
		}
	}

	PythonScriptingManager::~PythonScriptingManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_scripts.clear();
		Py_Finalize();
	}

	void PythonScriptingManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		while (!_callScripts.empty()) {
			std::function<void(void)> func;
			_callScripts.poll(func); // Daniel: no error check necessary as it is polled just from one thread
			func();
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
			loadAllScripts();
		} else {
			ISIXE_THROW_MESSAGE("PythonScriptingManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void PythonScriptingManager::loadAllScripts() {
		ASSERT_THREAD_SAFETY_FUNCTION
		std::queue<std::string> directories;
		directories.push(_scriptsPath);

		while (!directories.empty()) {
			std::string dir = directories.front();
			directories.pop();
			try {
				boost::filesystem::directory_iterator iter(dir), dirEnd;
				while (iter != dirEnd) {
					if (boost::filesystem::is_regular_file(*iter)) {
						std::string file = iter->path().string();
						parseScript(file, true);
					} else if (boost::filesystem::is_directory(*iter)) {
						std::string path = iter->path().string();
						directories.push(path);
					}
					iter++;
				}
			} catch (boost::filesystem::filesystem_error & e) {
				ISIXE_THROW_FAILURE("PythonScriptingManager", e.what());
			}
		}
	}

	void PythonScriptingManager::parseScript(const std::string & file, bool completePath) {
		ASSERT_THREAD_SAFETY_FUNCTION
		try {
			if (_scripts.find(file) == _scripts.end()) {
				std::string path = file;
				if (!completePath) {
					path = _scriptsPath + "/" + file + ".py";
				}
				boost::python::object module = boost::python::import("__main__");
				boost::python::object global = module.attr("__dict__");
				boost::python::exec_file(path.c_str(), global, global);
				_scripts.insert(file);
			}
		} catch (const boost::python::error_already_set &) {
			logStacktrace();
		}
	}

	void PythonScriptingManager::logStacktrace() {
		PyObject *exc, *val, *tb;
		PyErr_Fetch(&exc, &val, &tb);
		PyErr_NormalizeException(&exc, &val, &tb);
		boost::python::handle<> hexc(exc), hval(boost::python::allow_null(val)), htb(boost::python::allow_null(tb));
		if (!hval) {
			ISIXE_LOG_ERROR("PythonScriptingManager", std::string(boost::python::extract<std::string>(boost::python::str(hexc))));
		} else {
			boost::python::object traceback(boost::python::import("traceback"));
			boost::python::object format_exception(traceback.attr("format_exception"));
			boost::python::list formatted_list(format_exception(hexc, hval, htb));
			for (int count = 0; count < len(formatted_list); ++count) {
				ISIXE_LOG_ERROR("PythonScriptingManager", std::string(boost::python::extract<std::string>(formatted_list[count].slice(0, -1))));
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
