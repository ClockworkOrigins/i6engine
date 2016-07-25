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

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "components/DriveComponent.h"

namespace i6e {
namespace integration {

	MeshStriderShapeIntegrationTestApplication::MeshStriderShapeIntegrationTestApplication() : CommonApplication(true, false) {
	}

	MeshStriderShapeIntegrationTestApplication::~MeshStriderShapeIntegrationTestApplication() {
	}

	void MeshStriderShapeIntegrationTestApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		api::ObjectFacade * of = api::EngineController::GetSingleton().getObjectFacade();

		of->registerCTemplate("Drive", std::bind(&components::DriveComponent::createC, std::placeholders::_1, std::placeholders::_2));

		// a camera to see the scene and move around
		{
			api::objects::GOTemplate tmpl;
			{
				api::attributeMap params;
				params.insert(std::make_pair("pos", "0 50 -200"));

				tmpl._components.push_back(api::objects::GOTemplateComponent("StaticState", params, "", false, false));
			}
			of->createGO("SpectatorCam", tmpl, api::EngineController::GetSingleton().getUUID(), false, [this](api::GOPtr go) {
				_camera = go;
			});
		}
		// a ground so we can see shadows on it
		{
			api::objects::GOTemplate tmpl;
			{
				api::attributeMap params;
				params.insert(std::make_pair("mesh", "MeshStriderTestMesh.MESH"));
				params.insert(std::make_pair("scale", "1.0 1.0 1.0"));

				tmpl._components.push_back(api::objects::GOTemplateComponent("MeshAppearance", params, "", false, false));
			}
			{
				api::attributeMap params;
				params.insert(std::make_pair("shapeType", "3"));
				params.insert(std::make_pair("fileName", "../media/cshapes/MeshStriderTestShape.bullet"));

				tmpl._components.push_back(api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			of->createObject("Floor", tmpl, api::EngineController::GetSingleton().getUUID(), false);
		}
		// a sun
		{
			api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, api::EngineController::GetSingleton().getUUID(), false);
		}
		// first scripted block
		{
			api::objects::GOTemplate tmpl;
			{
				api::attributeMap params;
				params.insert(std::make_pair("pos", "10 1.0 0.0"));
				Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0).insertInMap("rot", params);

				tmpl._components.push_back(api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			{
				tmpl._components.push_back(api::objects::GOTemplateComponent("ScriptingShatter", api::attributeMap(), "", true, false));
			}
			{
				tmpl._components.push_back(api::objects::GOTemplateComponent("Drive", api::attributeMap(), "", false, false));
			}
			of->createObject("ScriptBlock", tmpl, api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace integration */
} /* namespace i6e */
