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
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6e::api::objects::GOTemplate tmpl;
			{
				i6e::api::attributeMap params;
				params.insert(std::make_pair("pos", "0 5 -20"));

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("StaticState", params, "", false, false));
			}
			of->createGO("SpectatorCam", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false, [this](i6e::api::GOPtr go) {
				_camera = go;
			});
		}
		{
			i6e::api::objects::GOTemplate tmpl;
			of->createObject("Floor", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		resetScene();

		i6e::api::InputFacade * inputFacade = i6e::api::EngineController::GetSingleton().getInputFacade();

		inputFacade->subscribeKeyEvent(i6e::api::KeyCode::KC_RETURN, i6e::api::KeyState::KEY_PRESSED, std::bind(&ChainApplication::resetScene, this));
	}

	void ChainApplication::resetScene() {
		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("ChainLink");
		of->deleteAllObjectsOfType("Plank");
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5.0, 0.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC["collisionGroup"] = "1 0 0";
			paramsPSC["mass"] = "0";

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank01";
				paramsConstraint["targetIdentifier"] = "ChainLink01";
				paramsConstraint["selfOffset"] = "0.0 -0.05 0.0";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));
			}

			of->createObject("Plank", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink01";
				paramsConstraint["targetIdentifier"] = "Plank01";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 -0.05 0.0";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink02";
				paramsConstraint["targetIdentifier"] = "ChainLink03";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.15, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink03";
				paramsConstraint["targetIdentifier"] = "ChainLink02";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink04";
				paramsConstraint["targetIdentifier"] = "ChainLink05";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.3, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink05";
				paramsConstraint["targetIdentifier"] = "ChainLink04";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink06";
				paramsConstraint["targetIdentifier"] = "ChainLink07";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.45, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink07";
				paramsConstraint["targetIdentifier"] = "ChainLink06";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink08";
				paramsConstraint["targetIdentifier"] = "ChainLink09";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.6, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink09";
				paramsConstraint["targetIdentifier"] = "ChainLink08";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink10";
				paramsConstraint["targetIdentifier"] = "ChainLink11";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.75, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink11";
				paramsConstraint["targetIdentifier"] = "ChainLink10";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink12";
				paramsConstraint["targetIdentifier"] = "ChainLink13";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.75, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink13";
				paramsConstraint["targetIdentifier"] = "ChainLink12";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink14";
				paramsConstraint["targetIdentifier"] = "ChainLink15";
				paramsConstraint["selfOffset"] = "0.0 0.0 -0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink02", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 5 - 0.075 - 0.9, 0.0);
			Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
			rot = rot * Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			rot.insertInMap("rot", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "ChainLink15";
				paramsConstraint["targetIdentifier"] = "ChainLink14";
				paramsConstraint["selfOffset"] = "0.0 0.0 0.075";
				paramsConstraint["targetOffset"] = "0.0 0.0 -0.075";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "ChainLink01", false, false));
			}

			of->createObject("ChainLink", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
