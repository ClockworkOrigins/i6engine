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

#include "i6engine/modules/luascripting/LuaScriptingManager.h"

#include <thread>

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ScriptingConfig.h"

extern "C"
{
	#include "i6engine/lua/lualib.h"
}

namespace i6engine {
namespace modules {

	LuaScriptingManager::LuaScriptingManager() : _luaState(luaL_newstate()), _parsedFiles(), _scriptsPath() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		luaopen_base(_luaState);
		luaopen_string(_luaState);
		luaopen_table(_luaState);
		luaopen_math(_luaState);
		luaopen_io(_luaState);
		luaopen_debug(_luaState);
		luabind::open(_luaState);

		std::cout << luaL_dostring(_luaState, "function add(first, second)\n"
								   "    return first + second;\n"
								   "end\n") << std::endl;

		std::cout << lua_getglobal(_luaState, "add") << std::endl;
		lua_pushnumber(_luaState, 3);
		lua_pushnumber(_luaState, 7);
		std::cout << lua_pcall(_luaState, 2, 1, 0) << std::endl;
		std::cout << lua_tointeger(_luaState, -1) << std::endl;

		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "LuaScriptsPath", _scriptsPath)) {
			ISIXE_LOG_ERROR("ScriptingController", "An exception has occurred: value LuaScriptsPath in section SCRIPT not found!");
			return;
		}
	}

	LuaScriptingManager::~LuaScriptingManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		lua_close(_luaState);
	}

	void LuaScriptingManager::News(const api::GameMessage::Ptr & msg) {
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
			ISIXE_THROW_MESSAGE("LuaScriptingManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	bool LuaScriptingManager::parseScript(const std::string & file) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_parsedFiles.find(file) == _parsedFiles.end()) {
			std::cout << "Loading: " << (_scriptsPath + "/" + file + ".lua") << std::endl;
			std::cout << "fetching add" << lua_getglobal(_luaState, "add") << std::endl;
			lua_pushnumber(_luaState, 3);
			lua_pushnumber(_luaState, 7);
			std::cout << lua_pcall(_luaState, 2, 1, 0) << std::endl;
			std::cout << lua_tointeger(_luaState, -1) << std::endl;
			int status = luaL_dofile(_luaState, (_scriptsPath + "/" + file + ".lua").c_str());
			std::cout << "fetching tick" << lua_getglobal(_luaState, "tick") << std::endl;
			std::cout << lua_pcall(_luaState, 0, 0, 0) << std::endl;
			std::cout << "status: " << status << std::endl;
			if (status) {
				return false;
			}
			_parsedFiles.insert(file);
		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
		return true;
	}

} /* namespace modules */
} /* namespace i6engine */
