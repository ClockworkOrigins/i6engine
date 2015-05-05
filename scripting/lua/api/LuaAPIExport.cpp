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

#include "i6engine/api/EngineController.h"

#include "i6engine/luabind/luabind.hpp"

namespace i6engine {
namespace lua {
namespace api {

	uint64_t getCurrentTime() {
		return i6engine::api::EngineController::GetSingleton().getCurrentTime();
	}

} /* namespace api */
} /* namespace lua */
} /* namespace i6engine */

extern "C" ISIXE_LUA_API int init(lua_State * L) {
	using namespace luabind;

	open(L);

	module(L)
		[
			def("getCurrentTime", &i6engine::lua::api::getCurrentTime)
		];

	return 0;
}
