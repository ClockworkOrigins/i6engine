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

#include "ChatApplication.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/utils/i6eRandom.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/input/InputController.h"
#include "i6engine/modules/network/NetworkController.h"

#ifdef ISIXE_WITH_CONSOLE
int main(int, char **) {
#else
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	sample::ChatApplication app("127.0.0.1", 12345, uint16_t(i6engine::utils::Random::GetSingleton().rand(12346, UINT16_MAX)));

	app.setName("Chat Client Sample");

	i6engine::api::EngineController::GetSingletonPtr()->registerApplication(app);

#ifdef ISIXE_WITH_CONSOLE
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6engine::modules::GraphicsController(), LNG_GRAPHICS_FRAME_TIME);
#else
	HWND hWnd = i6engine::api::EngineController::GetSingletonPtr()->createWindow(hInstance);

	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6engine::modules::GraphicsController(hWnd), LNG_GRAPHICS_FRAME_TIME);
#endif
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Input", new i6engine::modules::InputController(), LNG_INPUT_FRAME_TIME);
	i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Network", new i6engine::modules::NetworkController(), LNG_NETWORK_FRAME_TIME);

	i6engine::api::EngineController::GetSingletonPtr()->start();

	return 0;
}

