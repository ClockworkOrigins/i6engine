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

#include "MeshStriderShapeIntegrationTestApplication.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/input/InputController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

int main(int, char **) {
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6e::modules::GraphicsController(), { i6e::core::Subsystem::Object });
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new i6e::modules::ObjectController(), { i6e::core::Subsystem::Physic });
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Input", new i6e::modules::InputController(), LNG_INPUT_FRAME_TIME);
	i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new i6e::modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);

	i6e::integration::MeshStriderShapeIntegrationTestApplication app;

	app.setName("MeshStriderShape Test");

	i6e::api::EngineController::GetSingletonPtr()->registerApplication(app);

	i6e::api::EngineController::GetSingletonPtr()->start();

	return 0;
}
