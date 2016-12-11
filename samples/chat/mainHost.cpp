/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
	sample::ChatApplication app("127.0.0.1", 12345, 12345);

	app.setName("Chat Host Sample");

	i6eEngineController->registerApplication(app);

#ifdef ISIXE_WITH_CONSOLE
	i6eEngineController->registerSubSystem("Graphics", new i6e::modules::GraphicsController(), LNG_GRAPHICS_FRAME_TIME);
#else
	HWND hWnd = i6eEngineController->createWindow(hInstance);

	i6eEngineController->registerSubSystem("Graphics", new i6e::modules::GraphicsController(hWnd), LNG_GRAPHICS_FRAME_TIME);
#endif
	i6eEngineController->registerSubSystem("Input", new i6e::modules::InputController(), LNG_INPUT_FRAME_TIME);
	i6eEngineController->registerSubSystem("Network", new i6e::modules::NetworkController(), LNG_NETWORK_FRAME_TIME);

	i6eEngineController->start();

	return 0;
}

