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

#include "TerraformingApplication.h"

#include "i6engine/utils/i6eRandom.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/TerrainAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	TerraformingApplication::TerraformingApplication() : CommonApplication(true, false), _lastUpdateTime(i6engine::api::EngineController::GetSingleton().getCurrentTime()) {
	}

	TerraformingApplication::~TerraformingApplication() {
	}

	void TerraformingApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		// ambient light for the scene
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createGO("SpectatorCam", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this](i6engine::api::GOPtr go) {
				_camera = go;
			});
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			const size_t VERTICES = 65;
			i6engine::api::objects::GOTemplate tmpl;
			i6engine::api::attributeMap params;
			i6engine::api::writeAttribute(params, "size", 1200.0); // size of the terrain in world units
			i6engine::api::writeAttribute(params, "inputScale", 1.0); // scale factor for height in terrain
			i6engine::api::writeAttribute(params, "vertices", VERTICES); // vertices along one edge, must be 2^n + 1
			i6engine::api::writeAttribute(params, "layers", 2); // two texture layers
			i6engine::api::writeAttribute(params, "layer_0_size", 10); // size of a texture tile (reduce to get more repetition of the texture)
			i6engine::api::writeAttribute(params, "layer_0_diffusespecular", "GrassFloor.tga"); // should be a texture combined of a diffuse and a specular map
			i6engine::api::writeAttribute(params, "layer_0_normal", "GrassFloor.tga"); // should be a texture combined of a normal and a displacement map
			i6engine::api::writeAttribute(params, "layer_1_size", 30);
			i6engine::api::writeAttribute(params, "layer_1_diffusespecular", "Rock.tga");
			i6engine::api::writeAttribute(params, "layer_1_normal", "Rock.tga");
			i6engine::api::writeAttribute(params, "layer_1_minHeight", 7.0); // the minimum height this texture is added, not necessary for first layer because it marks a transition, so only starting from layer 2
			i6engine::api::writeAttribute(params, "layer_1_fadeDist", 2.0); // the fadedist declares how fast the blending between the two layers is done
			i6engine::api::writeAttribute(params, "minX", 0);
			i6engine::api::writeAttribute(params, "maxX", 0);
			i6engine::api::writeAttribute(params, "minY", 0);
			i6engine::api::writeAttribute(params, "maxY", 0);

			std::vector<std::vector<double>> heightdata(VERTICES, std::vector<double>(VERTICES, 0.0));

			for (size_t i = 0; i < VERTICES; i++) {
				for (size_t j = 0; j < VERTICES; j++) {
					heightdata[i][j] = i6engine::utils::Random::GetSingleton().rand(2001) / 100.0;
				}
			}

			i6engine::api::writeAttribute(params, "heightdata", heightdata);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("TerrainAppearance", params, "", false, false));

			of->createObject("Terrain", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);

			i6engine::api::InputFacade * inputFacade = i6engine::api::EngineController::GetSingleton().getInputFacade();

			inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_MBLeft, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&TerraformingApplication::lowerTerrain, this));
			inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_MBLeft, i6engine::api::KeyState::KEY_HOLD, boost::bind(&TerraformingApplication::lowerTerrain, this));
			inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_MBRight, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&TerraformingApplication::raiseTerrain, this));
			inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_MBRight, i6engine::api::KeyState::KEY_HOLD, boost::bind(&TerraformingApplication::raiseTerrain, this));
		}
	}

	void getHeight(const Vec3 & pos, uint64_t & x, uint64_t & z) {
		double edgeLength = 1200.0 / 65;
		x = uint64_t((pos.getX() + 600.0 + edgeLength / 2) / edgeLength - 0.5);
		z = 65 - uint64_t((pos.getZ() + 600.0 + edgeLength / 2) / edgeLength + 0.5);
	}

	void TerraformingApplication::lowerTerrain() {
		if (i6engine::api::EngineController::GetSingleton().getCurrentTime() - _lastUpdateTime >= 100000) {
			auto vec = i6engine::api::EngineController::GetSingleton().getGraphicsFacade()->getSelectables();
			for (size_t i = 0; i < vec.size(); i++) {
				if (vec[i].first == -1) {
					uint64_t x;
					uint64_t z;
					getHeight(vec[i].second, x, z);
					auto list = i6engine::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Terrain");
					auto tc = list[0]->getGOC<i6engine::api::TerrainAppearanceComponent>(i6engine::api::components::ComponentTypes::TerrainAppearanceComponent);
					tc->setHeightAtPosition(x, z, tc->getHeightAtPosition(x, z) - 0.1);
					break;
				}
			}
			_lastUpdateTime += 30000;
		}
	}

	void TerraformingApplication::raiseTerrain() {
		if (i6engine::api::EngineController::GetSingleton().getCurrentTime() - _lastUpdateTime >= 100000) {
			auto vec = i6engine::api::EngineController::GetSingleton().getGraphicsFacade()->getSelectables();
			for (size_t i = 0; i < vec.size(); i++) {
				if (vec[i].first == -1) {
					uint64_t x;
					uint64_t z;
					getHeight(vec[i].second, x, z);
					auto list = i6engine::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Terrain");
					auto tc = list[0]->getGOC<i6engine::api::TerrainAppearanceComponent>(i6engine::api::components::ComponentTypes::TerrainAppearanceComponent);
					tc->setHeightAtPosition(x, z, tc->getHeightAtPosition(x, z) + 0.1);
					break;
				}
			}
			_lastUpdateTime += 30000;
		}
	}

} /* namespace sample */
