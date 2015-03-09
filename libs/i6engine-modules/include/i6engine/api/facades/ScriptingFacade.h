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

namespace i6engine {
namespace api {

	class ISIXE_MODULES_API ScriptingFacade {
	public:
		ScriptingFacade();
		~ScriptingFacade();

		void callScript(const std::string & file, const std::string & func);

		void callScript(const std::string & file, const std::string & func, const int64_t id);
		void callScript(const std::string & file, const std::string & func, const int64_t ownID, const int64_t otherID);
		void callScript(const std::string & file, const std::string & func, const int64_t ownID, const double otherID);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

	private:
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
