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
 * \addtogroup PythonScripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_PYTHONSCRIPTINGMANAGER_H__
#define __I6ENGINE_MODULES_PYTHONSCRIPTINGMANAGER_H__

#include <set>

#include "i6engine/utils/Future.h"
#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "boost/python.hpp"

#include "clockUtils/container/DoubleBufferQueue.h"

namespace i6e {
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
		 * \brief loads all scripts into dictionary
		 */
		void loadAllScripts();

	private:
		std::set<std::string> _scripts;
		std::string _scriptsPath;
		clockUtils::container::DoubleBufferQueue<std::function<void(void)>, true, false> _callScripts;

		/**
		 * \brief called by PythonScriptingMailbox with a message
		 */
		void News(const api::GameMessage::Ptr & msg);

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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PYTHONSCRIPTINGMANAGER_H__ */

/**
 * @}
 */
