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

#include "ScriptingApplication.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "components/ScriptingShatterComponent.h"

namespace sample {

	ScriptingApplication::ScriptingApplication() : CommonApplication(true, false) {
	}

	ScriptingApplication::~ScriptingApplication() {
	}

	void ScriptingApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();

		of->registerCTemplate("ScriptingShatter", boost::bind(&components::ScriptingShatterComponent::createC, _1, _2));

		// a camera to see the scene and move around
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("pos", "0 5 -20"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("StaticState", params, "", false, false));
			}
			of->createGO("SpectatorCam", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this](i6engine::api::GOPtr go) {
				_camera = go;
			});
		}
		// a ground so we can see shadows on it
		{
			i6engine::api::objects::GOTemplate tmpl;
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

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			of->createObject("ScriptBlock", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("pos", "-10 1.0 0.0"));
				params.insert(std::make_pair("rot", "0.0 0.0 1.0 0.0"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			of->createObject("ScriptBlock", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
