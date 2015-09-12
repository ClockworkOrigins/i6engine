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

#include "MeshStriderShapeIntegrationTestApplication.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "components/DriveComponent.h"

namespace i6engine {
namespace integration {

	MeshStriderShapeIntegrationTestApplication::MeshStriderShapeIntegrationTestApplication() : CommonApplication(true, false) {
	}

	MeshStriderShapeIntegrationTestApplication::~MeshStriderShapeIntegrationTestApplication() {
	}

	void MeshStriderShapeIntegrationTestApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();

		of->registerCTemplate("Drive", boost::bind(&components::DriveComponent::createC, _1, _2));

		// a camera to see the scene and move around
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("pos", "0 50 -200"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("StaticState", params, "", false, false));
			}
			of->createGO("SpectatorCam", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this](i6engine::api::GOPtr go) {
				_camera = go;
			});
		}
		// a ground so we can see shadows on it
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("mesh", "MeshStriderTestMesh.MESH"));
				params.insert(std::make_pair("scale", "1.0 1.0 1.0"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("MeshAppearance", params, "", false, false));
			}
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("shapeType", "3"));
				params.insert(std::make_pair("fileName", "../media/cshapes/MeshStriderTestShape.bullet"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			of->createObject("Floor", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		// a sun
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		// first scripted block
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("pos", "10 1.0 0.0"));
				Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0).insertInMap("rot", params);

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			{
				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("ScriptingShatter", i6engine::api::attributeMap(), "", true, false));
			}
			{
				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Drive", i6engine::api::attributeMap(), "", false, false));
			}
			of->createObject("ScriptBlock", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace integration */
} /* namespace i6engine */
