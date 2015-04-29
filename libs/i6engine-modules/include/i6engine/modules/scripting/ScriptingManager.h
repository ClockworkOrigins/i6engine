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

#ifndef __I6ENGINE_MODULES_SCRIPTINGMANAGER_H__
#define __I6ENGINE_MODULES_SCRIPTINGMANAGER_H__

#include <map>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "boost/python.hpp"

namespace i6engine {
namespace api {
	struct RayTestResult;
} /* namespace api */
namespace modules {

	class ScriptingMailbox;

	class ScriptingManager {
		friend class ScriptingMailbox;

	public:
		/**
		 * \brief constructor
		 */
		ScriptingManager();

		/**
		 * \brief destructor
		 */
		~ScriptingManager();

	private:
		std::map<std::string, boost::python::object> _scripts;

		/**
		 * \brief called by ScriptingMailbox with a message
		 */
		void News(const api::GameMessage::Ptr & msg);

		/**
		 * \brief executes the given method in the given script
		 */
		template<typename... args>
		void callScript(const std::string & file, const std::string & func, args... B) {
			parseScript(file, func);

			try {
				boost::python::object f = _scripts[file][func];

				f(B...);
			} catch (const boost::python::error_already_set &) {
				PyErr_PrintEx(0);
			}
		}

		/**
		 * \brief parses the given script
		 */
		void parseScript(const std::string & file, const std::string & func);

		/**
		 * \brief forbidden
		 */
		ScriptingManager(const ScriptingManager &) = delete;

		/**
		 * \brief forbidden
		 */
		ScriptingManager & operator=(const ScriptingManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_SCRIPTINGMANAGER_H__ */

/**
 * @}
 */
