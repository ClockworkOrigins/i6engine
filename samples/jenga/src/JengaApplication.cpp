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

#include "JengaApplication.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	JengaApplication::JengaApplication() : CommonApplication(true, true) {
	}

	JengaApplication::~JengaApplication() {
	}

	void JengaApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		// ambient light for the scene
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		// setting shadow technique... currently only additive stencil possible
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6e::api::graphics::ShadowTechnique::Stencil_Additive);

		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6e::api::objects::GOTemplate tmpl;
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

		inputFacade->subscribeKeyEvent(i6e::api::KeyCode::KC_RETURN, i6e::api::KeyState::KEY_PRESSED, boost::bind(&JengaApplication::resetScene, this));
	}

	void JengaApplication::resetScene() {
		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("JengaStick");
		of->deleteAllObjectsOfType("Ball");
		// generate level
		for (uint32_t i = 0; i < 20; i++) { // rows
			for (int32_t j = 0; j < 3; j++) {
				i6e::api::objects::GOTemplate tmpl;

				Vec3 pos(0.0, 0.5 + 1.0 * i, (j - 1) * 2.5);
				Quaternion rot;

				if (i % 2 == 1) {
					pos.setZ(0.0);
					pos.setX((j - 1) * 2.5);
					rot = Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);
				}

				i6e::api::attributeMap paramsPSC;
				pos.insertInMap("pos", paramsPSC);
				rot.insertInMap("rot", paramsPSC);

				tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

				if (j == 1) {
					i6e::api::attributeMap paramsMesh;
					paramsMesh["material"] = "JengaStick_Middle";

					tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("MeshAppearance", paramsMesh, "", false, false));
				}

				of->createObject("JengaStick", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
			}
		}
	}

} /* namespace sample */
