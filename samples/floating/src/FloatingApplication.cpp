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

		inputFacade->subscribeKeyEvent(i6e::api::KeyCode::KC_RETURN, i6e::api::KeyState::KEY_PRESSED, std::bind(&FloatingApplication::resetScene, this));
	}

	void FloatingApplication::resetScene() {
		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("Ballon");
		of->deleteAllObjectsOfType("Plank");
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 0.05, 0.0);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank01";
				paramsConstraint["targetIdentifier"] = "Ballon01";
				paramsConstraint["selfOffset"] = "-1.5 0.05 -0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank01", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank02";
				paramsConstraint["targetIdentifier"] = "Ballon02";
				paramsConstraint["selfOffset"] = "-1.5 0.05 0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank02", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank03";
				paramsConstraint["targetIdentifier"] = "Ballon03";
				paramsConstraint["selfOffset"] = "1.5 0.05 -0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank03", false, false));
			}
			{
				i6e::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank04";
				paramsConstraint["targetIdentifier"] = "Ballon04";
				paramsConstraint["selfOffset"] = "1.5 0.05 0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank04", false, false));
			}

			of->createObject("Plank", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(-1.5, 0.45, -0.75);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6e::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon01";
			paramsConstraint["targetIdentifier"] = "Plank01";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "-1.5 0.05 -0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(-1.5, 0.45, 0.75);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6e::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon02";
			paramsConstraint["targetIdentifier"] = "Plank02";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "-1.5 0.05 0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(1.5, 0.45, -0.75);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6e::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon03";
			paramsConstraint["targetIdentifier"] = "Plank03";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "1.5 0.05 -0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6e::api::objects::GOTemplate tmpl;

			Vec3 pos(1.5, 0.45, 0.75);

			i6e::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6e::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon04";
			paramsConstraint["targetIdentifier"] = "Plank04";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "1.5 0.05 0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
