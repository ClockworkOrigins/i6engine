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

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "i6engine/luabind/luabind.hpp"

namespace i6engine {
namespace modules {

	class LuaScriptingMailbox;

	class LuaScriptingManager {
		friend class LuaScriptingMailbox;

	public:
		/**
		 * \brief constructor
		 */
		LuaScriptingManager();

		/**
		 * \brief destructor
		 */
		~LuaScriptingManager();

	private:
		lua_State * _luaState;
		std::set<std::string> _parsedFiles;

		/**
		 * \brief called by LuaScriptingMailbox with a message
		 */
		void News(const api::GameMessage::Ptr & msg);

		/**
		 * \brief executes the given method in the given script
		 */
		template<typename Ret, typename... args>
		Ret callScript(const std::string & file, const std::string & func, args... B) {
			ASSERT_THREAD_SAFETY_FUNCTION
			parseScript(file);
			return Ret(luabind::call_function<Ret>(_luaState, func.c_str(), B...));
		}

		/**
		 * \brief parses the given script
		 */
		void parseScript(const std::string & file);

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
