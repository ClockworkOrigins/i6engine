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

#include <iostream>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"

#include "TerrainShapeCreatorApplication.h"

#include "clockUtils/argParser/ArgumentParser.h"

int main(int argc, char ** argv) {
	REGISTER_VARIABLE(std::string, goTemplate, "", "Template of the terrain object the collision shape shall be created for");
	REGISTER_VARIABLE(std::string, shape, "", "The collision shape file where the export shalls go to including an optional path");

	if (PARSE_COMMANDLINE() != clockUtils::ClockError::SUCCESS) {
		std::cerr << GETLASTPARSERERROR() << std::endl;
		return 1;
	} else if (HELPSET()) {
		std::cout << GETHELPTEXT() << std::endl;
	} else if (!goTemplate.isSet()) {
		std::cerr << "Parameter 'goTemplate' not set!" << std::endl;
		return 1;
	} else if (!shape.isSet()) {
		std::cerr << "Parameter 'shape' not set!" << std::endl;
		return 1;
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

