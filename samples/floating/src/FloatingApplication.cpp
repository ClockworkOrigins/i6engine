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

#include "FloatingApplication.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	FloatingApplication::FloatingApplication() : CommonApplication(true, true) {
	}

	FloatingApplication::~FloatingApplication() {
	}

	void FloatingApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		// ambient light for the scene
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		// setting shadow technique... currently only additive stencil possible
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6engine::api::graphics::ShadowTechnique::Stencil_Additive);

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
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
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Floor", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		resetScene();

		i6engine::api::InputFacade * inputFacade = i6engine::api::EngineController::GetSingleton().getInputFacade();

		inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_RETURN, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&FloatingApplication::resetScene, this));
	}

	void FloatingApplication::resetScene() {
		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("Ballon");
		of->deleteAllObjectsOfType("Plank");
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 0.05, 0.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank01";
				paramsConstraint["targetIdentifier"] = "Ballon01";
				paramsConstraint["selfOffset"] = "-1.5 0.05 -0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank02";
				paramsConstraint["targetIdentifier"] = "Ballon02";
				paramsConstraint["selfOffset"] = "-1.5 0.05 0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank02", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank03";
				paramsConstraint["targetIdentifier"] = "Ballon03";
				paramsConstraint["selfOffset"] = "1.5 0.05 -0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank03", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank04";
				paramsConstraint["targetIdentifier"] = "Ballon04";
				paramsConstraint["selfOffset"] = "1.5 0.05 0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank04", false, false));
			}

			of->createObject("Plank", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(-1.5, 0.45, -0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon01";
			paramsConstraint["targetIdentifier"] = "Plank01";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "-1.5 0.05 -0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(-1.5, 0.45, 0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon02";
			paramsConstraint["targetIdentifier"] = "Plank02";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "-1.5 0.05 0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(1.5, 0.45, -0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon03";
			paramsConstraint["targetIdentifier"] = "Plank03";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "1.5 0.05 -0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(1.5, 0.45, 0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon04";
			paramsConstraint["targetIdentifier"] = "Plank04";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "1.5 0.05 0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
