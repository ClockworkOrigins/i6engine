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

#include "ShadowApplication.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/input/InputController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#ifdef ISIXE_WITH_CONSOLE
int main(int, char **) {
#else
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	sample::ShadowApplication app;

	app.setName("Shadow Sample");

	i6e::api::EngineController::GetSingletonPtr()->registerApplication(app);

#ifdef ISIXE_WITH_CONSOLE
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6e::modules::GraphicsController(), { i6e::core::Subsystem::Object });
#else
	HWND hWnd = i6e::api::EngineController::GetSingletonPtr()->createWindow(hInstance);

	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6e::modules::GraphicsController(hWnd), { i6e::core::Subsystem::Object });
#endif
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new i6e::modules::ObjectController(), { i6e::core::Subsystem::Physic });
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Input", new i6e::modules::InputController(), LNG_INPUT_FRAME_TIME);
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new i6e::modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);

	i6e::api::EngineController::GetSingletonPtr()->start();

	return 0;
}

