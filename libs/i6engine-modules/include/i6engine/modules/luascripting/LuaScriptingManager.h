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

/**
 * \addtogroup LuaScripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_LUASCRIPTINGMANAGER_H__
#define __I6ENGINE_MODULES_LUASCRIPTINGMANAGER_H__

#include <map>
#include <set>

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Future.h"
#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "i6engine/luabind/luabind.hpp"

#include "clockUtils/container/DoubleBufferQueue.h"

namespace i6e {
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

		/**
		 * \brief executes the given method in the given script with return type void
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

		/**
		 * \brief executes the given method in the given script with return type non-void
		 */
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

		/**
		 * \brief executes the given method with return type void
		 */
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

		/**
		 * \brief executes the given method with return type non-void
		 */
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

		/**
		 * \brief executes the given method in the given script with return type void and calls callback after scripts was executed
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScriptWithCallback(const std::string & file, const std::string & func, const std::function<void(void)> & callback, args... B) {
			_callScripts.push(std::bind([this, file, func, callback](args... A) {
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
				callback();
			}, B...));
		}

		/**
		 * \brief executes the given method in the given script with return type non-void and calls callback after scripts was executed
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callScriptWithCallback(const std::string & file, const std::string & func, const std::function<void(void)> & callback, args... B) {
			std::shared_ptr<utils::Future<Ret>> ret = std::make_shared<utils::Future<Ret>>();
			_callScripts.push(std::bind([this, file, func, callback, ret](args... A) {
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
				callback();
			}, B...));
			return ret;
		}

		/**
		 * \brief executes the given method with return type void and calls callback after scripts was executed
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callFunctionWithCallback(const std::string & func, const std::function<void(void)> & callback, args... B) {
			_callScripts.push(std::bind([this, func, callback](args... A) {
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
				callback();
			}, B...));
		}

		/**
		 * \brief executes the given method with return type non-void and calls callback after scripts was executed
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callFunctionWithCallback(const std::string & func, const std::function<void(void)> & callback, args... B) {
			std::shared_ptr<utils::Future<Ret>> ret = std::make_shared<utils::Future<Ret>>();
			_callScripts.push(std::bind([this, func, callback, ret](args... A) {
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
				callback();
			}, B...));
			return ret;
		}

		/**
		 * \brief sets a global variable
		 */
		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value>::type setGlobalVariable(const std::string & name, T value) {
			_callScripts.push(std::bind([this, name, value]() {
				ASSERT_THREAD_SAFETY_FUNCTION
				luabind::globals(_luaState)[name] = value;
			}));
		}

		/**
		 * \brief loads all scripts into dictionary
		 */
		void loadAllScripts();

	private:
		lua_State * _luaState;
		std::set<std::string> _parsedFiles;
		std::string _scriptsPath;
		clockUtils::container::DoubleBufferQueue<std::function<void(void)>, true, false> _callScripts;

		/**
		 * \brief called by LuaScriptingMailbox with a message
		 */
		void News(const api::GameMessage::Ptr & msg);

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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_LUASCRIPTINGMANAGER_H__ */

/**
 * @}
 */
