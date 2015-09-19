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

int main(int argc, char ** argv) {
	argc--;
	argv++;

	if (argc != 2) {
		ISIXE_THROW_API("terrainShapeCreator", "Wrong parameters: goTemplate out.bullet");
	} else {
		std::string goTemplate = argv[0];
		std::string bullet = argv[1];

		std::cout << "Initializing i6engine" << std::endl;

		i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new i6engine::modules::GraphicsController(), 10000);
		i6engine::api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new i6engine::modules::ObjectController(), 10000);

		i6engine::tools::TerrainShapeCreatorApplication app(goTemplate, bullet);

		app.setName("TerrainShapeCreator");

		i6engine::api::EngineController::GetSingletonPtr()->registerApplication(app);

		i6engine::api::EngineController::GetSingletonPtr()->start();
	}

	return 0;
}

