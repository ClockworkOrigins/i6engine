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

#include "ScriptingApplication.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/input/InputController.h"
#include "i6engine/modules/network/NetworkController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#if ISIXE_SCRIPTING == SCRIPTING_LUA
	#include "i6engine/modules/luascripting/LuaScriptingController.h"
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
	#include "i6engine/modules/pythonscripting/PythonScriptingController.h"
#endif

int main(int argc, char ** argv) {
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6engine::modules::GraphicsController(), { i6engine::core::Subsystem::Object });
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new i6engine::modules::ObjectController(), { i6engine::core::Subsystem::Physic });
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Input", new i6engine::modules::InputController(), LNG_INPUT_FRAME_TIME);
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new i6engine::modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Network", new i6engine::modules::NetworkController(), LNG_NETWORK_FRAME_TIME);
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new i6engine::modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#if ISIXE_SCRIPTING == SCRIPTING_LUA
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Scripting", new i6engine::modules::LuaScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Scripting", new i6engine::modules::PhytonScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#endif

	sample::ScriptingApplication app;

	app.setName("Scripting Sample");

	i6engine::api::EngineController::GetSingletonPtr()->registerApplication(app);

	i6engine::api::EngineController::GetSingletonPtr()->start();

	return 0;
}

