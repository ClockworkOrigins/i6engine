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
	#include "i6engine/modules/pythonscripting/PythonScriptingManager.h"
#endif

namespace i6e {
namespace api {

	class ISIXE_MODULES_API ScriptingFacade {
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		friend class modules::LuaScriptingManager;
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		friend class modules::PythonScriptingManager;
#endif

	public:
		ScriptingFacade();
		~ScriptingFacade();

		/**
		 * \brief preloads all scripts in the in i6engine.ini specified script directory
		 */
		void loadAllScripts() const;

#if ISIXE_SCRIPTING != SCRIPTING_NONE
		/**
		 * \brief runs method in script with void return type
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScript(const std::string & file, const std::string & func, args... B) {
			_manager->callScript<Ret>(file, func, B...);
		}

		/**
		 * \brief runs method in script with non-void return type
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callScript(const std::string & file, const std::string & func, args... B) {
			return _manager->callScript<Ret>(file, func, B...);
		}

		/**
		 * \brief runs method with void return type
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callFunction(const std::string & func, args... B) {
			_manager->callFunction<Ret>(func, B...);
		}

		/**
		 * \brief runs method with non-void return type
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callFunction(const std::string & func, args... B) {
			return _manager->callFunction<Ret>(func, B...);
		}

		/**
		 * \brief runs method in script with void return type and calls callback after execution of the method
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScriptWithCallback(const std::string & file, const std::string & func, const std::function<void(void)> & callback, args... B) {
			_manager->callScriptWithCallback<Ret>(file, func, callback, B...);
		}

		/**
		 * \brief runs method in script with non-void return type and calls callback after execution of the method
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callScriptWithCallback(const std::string & file, const std::string & func, const std::function<void(void)> & callback, args... B) {
			return _manager->callScriptWithCallback<Ret>(file, func, callback, B...);
		}

		/**
		 * \brief runs method with void return type and calls callback after execution of the method
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callFunctionWithCallback(const std::string & func, const std::function<void(void)> & callback, args... B) {
			_manager->callFunctionWithCallback<Ret>(func, callback, B...);
		}

		/**
		 * \brief runs method with non-void return type and calls callback after execution of the method
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callFunctionWithCallback(const std::string & func, const std::function<void(void)> & callback, args... B) {
			return _manager->callFunction<Ret>(func, callback, B...);
		}

		/**
		 * \brief sets a global variable
		 */
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
		modules::PythonScriptingManager * _manager;
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
} /* namespace i6e */

#endif /* __I6ENGINE_API_SCRIPTINGFACADE_H__ */

/**
 * @}
 */
