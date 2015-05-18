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

/**
 * \addtogroup LuaScripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_LUASCRIPTINGMANAGER_H__
#define __I6ENGINE_MODULES_LUASCRIPTINGMANAGER_H__

#include <map>
#include <set>

#include "i6engine/utils/DoubleBufferQueue.h"
#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Future.h"
#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "i6engine/luabind/luabind.hpp"

namespace i6engine {
namespace api {
	class ScriptingFacade;
} /* namespace api */
namespace modules {

	class LuaScriptingMailbox;

	class ISIXE_MODULES_API LuaScriptingManager {
		friend class LuaScriptingMailbox;
		friend class api::ScriptingFacade;

	public:
		/**
		 * \brief constructor
		 */
		LuaScriptingManager();

		/**
		 * \brief destructor
		 */
		~LuaScriptingManager();

		void Tick();

	private:
		lua_State * _luaState;
		std::set<std::string> _parsedFiles;
		std::string _scriptsPath;
		utils::DoubleBufferQueue<std::function<void(void)>, true, false> _callScripts;

		/**
		 * \brief called by LuaScriptingMailbox with a message
		 */
		void News(const api::GameMessage::Ptr & msg);

		/**
		 * \brief executes the given method in the given script
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScript(const std::string & file, const std::string & func, args... B) {
			_callScripts.push(std::bind([this, file, func](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				if (!parseScript(file, false)) {
					return;
				}
				try {
					lua_getglobal(_luaState, func.c_str());
					assert(lua_isfunction(_luaState, -1));
					luabind::object o(luabind::from_stack(_luaState, -1));
					luabind::call_function<Ret>(o, A...);
				} catch (const luabind::error & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "' in script '" << file << ".lua': " << e.what() << ": " << lua_tostring(_luaState, -1));
				} catch (const std::exception & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "' in script '" << file << ".lua': " << e.what() << ": " << lua_tostring(_luaState, -1));
				}
			}, B...));
		}

		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callScript(const std::string & file, const std::string & func, args... B) {
			std::shared_ptr<utils::Future<Ret>> ret = std::make_shared<utils::Future<Ret>>();
			_callScripts.push(std::bind([this, file, func, ret](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				if (!parseScript(file, false)) {
					ret->push(Ret());
				}
				try {
					lua_getglobal(_luaState, func.c_str());
					assert(lua_isfunction(_luaState, -1));
					luabind::object o(luabind::from_stack(_luaState, -1));
					ret->push(Ret(luabind::call_function<Ret>(o, A...)));
				} catch (const luabind::error & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "' in script '" << file << ".lua': " << e.what() << ": " << lua_tostring(_luaState, -1));
				} catch (const std::exception & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "' in script '" << file << ".lua': " << e.what() << ": " << lua_tostring(_luaState, -1));
				}
			}, B...));
			return ret;
		}

		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callFunction(const std::string & func, args... B) {
			_callScripts.push(std::bind([this, func](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				try {
					lua_getglobal(_luaState, func.c_str());
					assert(lua_isfunction(_luaState, -1));
					luabind::object o(luabind::from_stack(_luaState, -1));
					luabind::call_function<Ret>(o, A...);
				} catch (const luabind::error & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "': " << e.what() << ": " << lua_tostring(_luaState, -1));
				} catch (const std::exception & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "': " << e.what() << ": " << lua_tostring(_luaState, -1));
				}
			}, B...));
		}

		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callFunction(const std::string & func, args... B) {
			std::shared_ptr<utils::Future<Ret>> ret = std::make_shared<utils::Future<Ret>>();
			_callScripts.push(std::bind([this, func, ret](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				try {
					lua_getglobal(_luaState, func.c_str());
					assert(lua_isfunction(_luaState, -1));
					luabind::object o(luabind::from_stack(_luaState, -1));
					ret->push(Ret(luabind::call_function<Ret>(o, A...)));
				} catch (const luabind::error & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "': " << e.what() << ": " << lua_tostring(_luaState, -1));
				} catch (const std::exception & e) {
					ISIXE_THROW_FAILURE("LuaScriptingManager", "Error calling function '" << func << "': " << e.what() << ": " << lua_tostring(_luaState, -1));
				}
			}, B...));
			return ret;
		}

		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value>::type setGlobalVariable(const std::string & name, T value) {
			_callScripts.push(std::bind([this, name, value]() {
				ASSERT_THREAD_SAFETY_FUNCTION
				luabind::globals(_luaState)[name] = value;
			}));
		}

		/**
		 * \brief parses the given script
		 */
		bool parseScript(const std::string & file, bool completePath);

		/**
		 * \brief forbidden
		 */
		LuaScriptingManager(const LuaScriptingManager &) = delete;

		/**
		 * \brief forbidden
		 */
		LuaScriptingManager & operator=(const LuaScriptingManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_LUASCRIPTINGMANAGER_H__ */

/**
 * @}
 */
