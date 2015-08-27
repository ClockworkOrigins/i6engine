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
 * \addtogroup PythonScripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_PYTHONSCRIPTINGMANAGER_H__
#define __I6ENGINE_MODULES_PYTHONSCRIPTINGMANAGER_H__

#include <set>

#include "i6engine/utils/DoubleBufferQueue.h"
#include "i6engine/utils/Future.h"
#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "boost/python.hpp"

namespace i6engine {
namespace api {
	class ScriptingFacade;
} /* namespace api */
namespace modules {

	class PythonScriptingMailbox;

	class ISIXE_MODULES_API PythonScriptingManager {
		friend class PythonScriptingMailbox;
		friend class api::ScriptingFacade;

	public:
		/**
		 * \brief constructor
		 */
		PythonScriptingManager();

		/**
		 * \brief destructor
		 */
		~PythonScriptingManager();

		void Tick();

	private:
		std::set<std::string> _scripts;
		std::string _scriptsPath;
		utils::DoubleBufferQueue<std::function<void(void)>, true, false> _callScripts;

		/**
		 * \brief called by PythonScriptingMailbox with a message
		 */
		void News(const api::GameMessage::Ptr & msg);

		/**
		 * \brief executes the given method in the given script with return type void
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScript(const std::string & file, const std::string & func, args... B) {
			_callScripts.push(std::bind([this, file, func](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				parseScript(file, false);

				try {
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					boost::python::call<Ret>(f.ptr(), A...);
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
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
				parseScript(file, false);

				try {
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					ret->push(boost::python::call<Ret>(f.ptr(), A...));
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
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
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					boost::python::call<Ret>(f.ptr(), A...);
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
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
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					ret->push(boost::python::call<Ret>(f.ptr(), A...));
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
				}
			}, B...));
			return ret;
		}

		/**
		 * \brief executes the given method in the given script with return type void and calls callback after execution of the script
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callScriptWithCallback(const std::string & file, const std::string & func, const std::function<void(void)> & callback, args... B) {
			_callScripts.push(std::bind([this, file, func, callback](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				parseScript(file, false);
				try {
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					boost::python::call<Ret>(f.ptr(), A...);
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
				}
				callback();
			}, B...));
		}

		/**
		 * \brief executes the given method in the given script with return type non-void and calls callback after execution of the script
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callScriptWithCallback(const std::string & file, const std::string & func, const std::function<void(void)> & callback, args... B) {
			std::shared_ptr<utils::Future<Ret>> ret = std::make_shared<utils::Future<Ret>>();
			_callScripts.push(std::bind([this, file, func, callback, ret](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				parseScript(file, false);
				try {
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					ret->push(boost::python::call<Ret>(f.ptr(), A...));
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
				}
				callback();
			}, B...));
			return ret;
		}

		/**
		 * \brief executes the given method with return type void and calls callback after execution of the script
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<std::is_void<Ret>::value, Ret>::type callFunctionWithCallback(const std::string & func, const std::function<void(void)> & callback, args... B) {
			_callScripts.push(std::bind([this, func, callback](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				try {
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					boost::python::call<Ret>(f.ptr(), A...);
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
				}
				callback();
			}, B...));
		}

		/**
		 * \brief executes the given method with return type non-void and calls callback after execution of the script
		 */
		template<typename Ret, typename... args>
		typename std::enable_if<!std::is_void<Ret>::value, std::shared_ptr<utils::Future<Ret>>>::type callFunctionWithCallback(const std::string & func, const std::function<void(void)> & callback, args... B) {
			std::shared_ptr<utils::Future<Ret>> ret = std::make_shared<utils::Future<Ret>>();
			_callScripts.push(std::bind([this, func, callback, ret](args... A) {
				ASSERT_THREAD_SAFETY_FUNCTION
				try {
					boost::python::object module = boost::python::import("__main__");
					boost::python::object global = module.attr("__dict__");
					boost::python::object f = global[func];
					ret->push(boost::python::call<Ret>(f.ptr(), A...));
				} catch (const boost::python::error_already_set &) {
					logStacktrace();
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
				boost::python::scope().attr(name.c_str()) = value;
			}));
		}

		/**
		 * \brief parses the given script
		 */
		void parseScript(const std::string & file, bool completePath);

		/**
		 * \brief log stacktrace
		 */
		void logStacktrace();

		/**
		 * \brief forbidden
		 */
		PythonScriptingManager(const PythonScriptingManager &) = delete;

		/**
		 * \brief forbidden
		 */
		PythonScriptingManager & operator=(const PythonScriptingManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_PYTHONSCRIPTINGMANAGER_H__ */

/**
 * @}
 */
