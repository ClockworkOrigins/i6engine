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

#include "boost/python/object.hpp"

namespace i6engine {
namespace api {
	struct RayTestResult;
} /* api */
namespace modules {

	class ScriptingMailbox;
	class ScriptingController;

	class ScriptingManager {
		friend class ScriptingMailbox;

	public:
		/**
		 * \brief constructor
		 */
		ScriptingManager(ScriptingController * ctrl);

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
		void callScript(const std::string & file, const std::string & func);

		/**
		 * \brief executes the given method in the given script with parameter
		 */
		void callScript(const std::string & file, const std::string & func, const int64_t id);
		void callScript(const std::string & file, const std::string & func, const int64_t id, const int64_t id2);
		void callScript(const std::string & file, const std::string & func, const int64_t id, const double d);

		/**
		 * \brief executes the given method in the given script with parameter
		 */
		void callScript(const std::string & file, const std::string & func, const api::RayTestResult & rtr, const int64_t rayID);

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

		ScriptingController * _ctrl;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_SCRIPTINGMANAGER_H__ */

/**
 * @}
 */
