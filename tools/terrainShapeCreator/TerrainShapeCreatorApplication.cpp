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

#include "TerrainShapeCreatorApplication.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/TerrainAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace tools {

	void TerrainShapeCreatorApplication::Initialize() {
	}

	void TerrainShapeCreatorApplication::AfterInitialize() {
		std::cout << "Loading Template '" << _goTemplate << "'" << std::endl;
		api::EngineController::GetSingleton().getObjectFacade()->createGO(_goTemplate, api::objects::GOTemplate(), api::EngineController::GetSingleton().getUUID(), false, [this](api::GOPtr go) {
			go->getGOC<api::TerrainAppearanceComponent>(api::components::ComponentTypes::TerrainAppearanceComponent)->saveCollisionShape(_outFile, std::bind(&api::EngineController::stop, api::EngineController::GetSingletonPtr()));
		});
	}

} /* namespace tools */
} /* namespace i6engine */
