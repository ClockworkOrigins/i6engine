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

#include "GUIAddImagesIntegrationTestApplication.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/modules/graphics/GraphicsController.h"

int main(int, char **) {
	i6eEngineController->registerSubSystem("Graphics", new i6e::modules::GraphicsController(), LNG_GRAPHICS_FRAME_TIME);

	i6e::integration::GUIAddImagesIntegrationTestApplication app;

	app.setName("GUIAddImages Test");

	i6eEngineController->registerApplication(app);

	i6eEngineController->start();

	return 0;
}
