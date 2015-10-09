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

#include "TerrainApplication.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	TerrainApplication::TerrainApplication() : CommonApplication(true, true) {
	}

	TerrainApplication::~TerrainApplication() {
	}

	void TerrainApplication::AfterInitialize() {
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
			i6engine::api::objects::GOTemplate tmpl;
			i6engine::api::attributeMap params;
			params.insert(std::make_pair("size", "1200.0")); // size of the terrain in world units
			params.insert(std::make_pair("heightmap", "terrain.png")); // the heightmap used for terrain generation
			params.insert(std::make_pair("inputScale", "60.0")); // scale factor for height in terrain (so maximum height here is 60 meters)
			params.insert(std::make_pair("vertices", "65")); // vertices along one edge, must be 2^n + 1
			params.insert(std::make_pair("layers", "2")); // two texture layers
			params.insert(std::make_pair("layer_0_size", "10")); // size of a texture tile (reduce to get more repetition of the texture)
			params.insert(std::make_pair("layer_0_diffusespecular", "GrassFloor.tga")); // should be a texture combined of a diffuse and a specular map
			params.insert(std::make_pair("layer_0_normal", "GrassFloor.tga")); // should be a texture combined of a normal and a displacement map
			params.insert(std::make_pair("layer_1_size", "30"));
			params.insert(std::make_pair("layer_1_diffusespecular", "Rock.tga"));
			params.insert(std::make_pair("layer_1_normal", "Rock.tga"));
			params.insert(std::make_pair("layer_1_minHeight", "7.0")); // the minimum height this texture is added, not necessary for first layer because it marks a transition, so only starting from layer 2
			params.insert(std::make_pair("layer_1_fadeDist", "2.0")); // the fadedist declares how fast the blending between the two layers is done
			params.insert(std::make_pair("minX", "-2")); // the terrain consists of 20 terrain parts
			params.insert(std::make_pair("maxX", "2"));
			params.insert(std::make_pair("minY", "-2"));
			params.insert(std::make_pair("maxY", "2"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("TerrainAppearance", params, "", false, false));

			of->createObject("Terrain", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
