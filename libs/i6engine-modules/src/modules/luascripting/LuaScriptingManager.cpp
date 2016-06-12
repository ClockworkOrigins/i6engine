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

#include "i6engine/modules/luascripting/LuaScriptingManager.h"

#include <thread>

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ScriptingConfig.h"
#include "i6engine/api/facades/ScriptingFacade.h"

extern "C"
{
	#include "i6engine/lua/lualib.h"
}

#include "boost/filesystem.hpp"

namespace i6e {
namespace modules {

	LuaScriptingManager::LuaScriptingManager() : _luaState(luaL_newstate()), _parsedFiles(), _scriptsPath(), _callScripts() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		luaL_openlibs(_luaState);
		//luabind::open(_luaState);
		ISIXE_LOG_INFO("LuaScriptingManager", LUA_COPYRIGHT);

		// load i6engine exports once during startup, so user hasn't to
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		int status = luaL_dostring(_luaState, "package.loadlib('ScriptingLua','init')()");
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		int status = luaL_dostring(_luaState, "package.loadlib('libScriptingLua.so','init')()");
#endif
		if (status != 0) {
			ISIXE_THROW_FAILURE("LuaScriptingManager", "Can't load ScriptingLua library. Lua reports: " << lua_tostring(_luaState, -1));
		}

		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "LuaScriptsPath", _scriptsPath)) {
			ISIXE_THROW_FAILURE("LuaScriptingManager", "An exception has occurred: value LuaScriptsPath in section SCRIPT not found!");
		}
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		api::EngineController::GetSingleton().getScriptingFacade()->_manager = this;
#endif
	}

	LuaScriptingManager::~LuaScriptingManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		lua_close(_luaState);
	}

	void LuaScriptingManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		while (!_callScripts.empty()) {
			std::function<void(void)> func;
			_callScripts.poll(func); // Daniel: no error check necessary as it is polled just from one thread
			func();
		}
		// after all scripts have been called collect garbage!
		int status = luaL_dostring(_luaState, "collectgarbage('collect')");
		if (status != 0) {
			ISIXE_LOG_WARN("LuaScriptingManager", "Collect garbage reported some error. Lua reports: " << lua_tostring(_luaState, -1));
		}
	}

	void LuaScriptingManager::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::scripting::ScrRayResult) {
			std::string file = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->file;
			std::string func = static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->func;

			callScript<void>(file, func, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->raytestResult, static_cast<api::scripting::Scripting_RayResult_Update *>(msg->getContent())->rayID);
		} else if (type == api::scripting::ScrLoadAllScripts) {
			loadAllScripts();
		} else {
			ISIXE_THROW_MESSAGE("LuaScriptingManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void LuaScriptingManager::loadAllScripts() {
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

	bool LuaScriptingManager::parseScript(const std::string & file, bool completePath) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_parsedFiles.find(file) == _parsedFiles.end()) {
			std::string path = file;
			if (!completePath) {
				path = _scriptsPath + "/" + file + ".lua";
			}
			int status = luaL_dofile(_luaState, path.c_str());
			if (status) {
				ISIXE_THROW_FAILURE("LuaScriptingManager", "Error parsing script '" << path << "': " << lua_tostring(_luaState, -1));
				return false;
			}
			_parsedFiles.insert(file);
		}
		return true;
	}

} /* namespace modules */
} /* namespace i6e */
