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

#include <iostream>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"

#include "TerrainShapeCreatorApplication.h"

#ifdef max
	#undef max
#endif

#include "clockUtils/argParser/ArgumentParser.h"

int main(int argc, char ** argv) {
	REGISTER_VARIABLE_REQUIRED(std::string, goTemplate, g, "", "Template of the terrain object the collision shape shall be created for");
	REGISTER_VARIABLE_REQUIRED(std::string, shape, s, "", "The collision shape file where the export shalls go to including an optional path");

	if (PARSE_COMMANDLINE() != clockUtils::ClockError::SUCCESS) {
		std::cerr << GETLASTPARSERERROR() << std::endl;
		return 1;
	} else if (HELPSET()) {
		std::cout << GETHELPTEXT() << std::endl;
	} else {
		std::cout << "Initializing i6engine" << std::endl;

		i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6e::modules::GraphicsController(), 10000);
		i6e::api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new i6e::modules::ObjectController(), 10000);

		i6e::tools::TerrainShapeCreatorApplication app(goTemplate, shape);

		app.setName("TerrainShapeCreator");

		i6e::api::EngineController::GetSingletonPtr()->registerApplication(app);

		i6e::api::EngineController::GetSingletonPtr()->start();
	}

	return 0;
}

