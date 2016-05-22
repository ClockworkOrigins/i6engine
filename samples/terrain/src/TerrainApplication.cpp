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
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6e::api::objects::GOTemplate tmpl;
			of->createGO("SpectatorCam", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false, [this](i6e::api::GOPtr go) {
				_camera = go;
			});
		}
		{
			i6e::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;
			i6e::api::attributeMap params;
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

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("TerrainAppearance", params, "", false, false));

			of->createObject("Terrain", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
