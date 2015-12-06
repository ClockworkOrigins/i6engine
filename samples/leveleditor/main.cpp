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

#include "LeveleditorApplication.h"

#include "i6engine/api/EngineController.h"

#ifdef ISIXE_WITH_CONSOLE
int main(int argc, char ** argv) {
#else
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	sample::LeveleditorApplication app;

	app.setName("Leveleditor Sample");

	i6engine::api::EngineController::GetSingletonPtr()->registerApplication(app);

#ifdef ISIXE_WITH_CONSOLE
	i6engine::api::EngineController::GetSingletonPtr()->registerDefault(false);
#else
	HWND hWnd = i6engine::api::EngineController::GetSingletonPtr()->createWindow(hInstance);

	i6engine::api::EngineController::GetSingletonPtr()->registerDefault(false, hWnd);
#endif

	i6engine::api::EngineController::GetSingletonPtr()->start();

	return 0;
}

