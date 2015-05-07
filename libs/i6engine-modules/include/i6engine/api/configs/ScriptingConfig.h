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

#ifndef __I6ENGINE_API_SCRIPTINGCONFIG_H__
#define __I6ENGINE_API_SCRIPTINGCONFIG_H__

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/configs/PhysicsConfig.h"

namespace i6engine {
namespace api {
namespace scripting {
	enum ScriptingMessageTypes {
		ScrReset = 0,
		ScrRayResult
	};

	/**
	 * \brief struct for message executing given method in given script file using raytest result as parameter
	 */
	typedef struct ISIXE_MODULES_API Scripting_RayResult_Update : physics::Physics_RayTestNotify_Update {
		std::string file;
		std::string func;
		int64_t rayID;

		Scripting_RayResult_Update(const std::string & fi, const std::string & fu, const int64_t & ri);
		Scripting_RayResult_Update * copy() {
			return new Scripting_RayResult_Update(*this);
		}
	} Scripting_RayResult_Update;

} /* namespace scripting */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_SCRIPTINGCONFIG_H__ */

/**
 * @}
 */
