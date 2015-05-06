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

#include "LuaAPIExport.h"
#include "LuaAudioExport.h"
#include "LuaGUIExport.h"
#include "LuaMathExport.h"
#include "LuaObjectExport.h"

extern "C" ISIXE_LUA_API int init(lua_State * L) {
	using namespace luabind;

	open(L);

	module(L)
		[
			registerAPI(),
			registerAudio(),
			registerGUI(),
			registerMath(),
			registerObject()
		];

	return 0;
}