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
 * \addtogroup Scripting
 * @{
 */

#ifndef __I6ENGINE_API_SCRIPTINGFACADE_H__
#define __I6ENGINE_API_SCRIPTINGFACADE_H__

#include <cstdint>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

#if ISIXE_SCRIPTING == SCRIPTING_LUA
	#include "i6engine/modules/luascripting/LuaScriptingManager.h"
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
	#include "i6engine/modules/scripting/ScriptingManager.h"
#endif

namespace i6engine {
namespace api {

	class ISIXE_MODULES_API ScriptingFacade {
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		friend class modules::LuaScriptingManager;
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		friend class modules::ScriptingManager;
#endif

	public:
		ScriptingFacade();
		~ScriptingFacade();

		void loadAllScripts() const;

#if ISIXE_SCRIPTING != SCRIPTING_NONE
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScript(const std::string & file, const std::string & func, args... B) {
			_manager->callScript<Ret>(file, func, B...);
		}

		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callScript(const std::string & file, const std::string & func, args... B) {
			return _manager->callScript<Ret>(file, func, B...);
		}

		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callFunction(const std::string & func, args... B) {
			_manager->callFunction<Ret>(func, B...);
		}

		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callFunction(const std::string & func, args... B) {
			return _manager->callFunction<Ret>(func, B...);
		}

		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value>::type setGlobalVariable(const std::string & name, T value) {
			_manager->setGlobalVariable(name, value);
		}
#endif

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

	private:
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		modules::LuaScriptingManager * _manager;
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		modules::ScriptingManager * _manager;
#endif

		/**
		 * \brief forbidden
		 */
		ScriptingFacade(const ScriptingFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		ScriptingFacade & operator=(const ScriptingFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_SCRIPTINGFACADE_H__ */

/**
 * @}
 */
