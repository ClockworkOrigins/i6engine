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
		luaL_openlibs(_luaState);
		ISIXE_LOG_INFO("LuaScriptingManager", LUA_COPYRIGHT);

		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "LuaScriptsPath", _scriptsPath)) {
			ISIXE_THROW_FAILURE("LuaScriptingController", "An exception has occurred: value LuaScriptsPath in section SCRIPT not found!");
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
			int status = luaL_dofile(_luaState, (_scriptsPath + "/" + file + ".lua").c_str());
			if (status) {
				ISIXE_THROW_FAILURE("LuaScriptingManager", "Error parsing script '" << file << ".lua': " << lua_tostring(_luaState, -1));
				return false;
			}
			_parsedFiles.insert(file);
		}
		return true;
	}

} /* namespace modules */
} /* namespace i6engine */
