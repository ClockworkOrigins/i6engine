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

#include "ChainApplication.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	ChainApplication::ChainApplication() : CommonApplication(true, true) {
	}

	ChainApplication::~ChainApplication() {
	}

	void ChainApplication::AfterInitialize() {
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

		inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_RETURN, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&ChainApplication::resetScene, this));
	}

	void ChainApplication::resetScene() {
		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("ChainLink");
		of->deleteAllObjectsOfType("Plank");
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5.0, 0.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC["collisionGroup"] = "1 0 0";
			paramsPSC["mass"] = "0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank01";
				paramsConstraint["targetIdentifier"] = "ChainLink01";
				paramsConstraint["selfOffset"] = "0.0 -0.05 0.0";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));
			}

			of->createObject("Plank", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink01";
				paramsConstraint["targetIdentifier"] = "Plank01";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 -0.05 0.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink02";
				paramsConstraint["targetIdentifier"] = "ChainLink03";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.15, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink03";
				paramsConstraint["targetIdentifier"] = "ChainLink02";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink04";
				paramsConstraint["targetIdentifier"] = "ChainLink05";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.3, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink05";
				paramsConstraint["targetIdentifier"] = "ChainLink04";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink06";
				paramsConstraint["targetIdentifier"] = "ChainLink07";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.45, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink07";
				paramsConstraint["targetIdentifier"] = "ChainLink06";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink08";
				paramsConstraint["targetIdentifier"] = "ChainLink09";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.6, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink09";
				paramsConstraint["targetIdentifier"] = "ChainLink08";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink10";
				paramsConstraint["targetIdentifier"] = "ChainLink11";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.75, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink11";
				paramsConstraint["targetIdentifier"] = "ChainLink10";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink12";
				paramsConstraint["targetIdentifier"] = "ChainLink13";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.75, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink13";
				paramsConstraint["targetIdentifier"] = "ChainLink12";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink14";
				paramsConstraint["targetIdentifier"] = "ChainLink15";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.9, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink15";
				paramsConstraint["targetIdentifier"] = "ChainLink14";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}

			of->createObject("ChainLink", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
