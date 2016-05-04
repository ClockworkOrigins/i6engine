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

#include "PhysicsPowerApplication.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	PhysicsPowerApplication::PhysicsPowerApplication() : CommonApplication(true, true) {
	}

	PhysicsPowerApplication::~PhysicsPowerApplication() {
	}

	void PhysicsPowerApplication::AfterInitialize() {
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

		inputFacade->subscribeKeyEvent(i6e::api::KeyCode::KC_RETURN, i6e::api::KeyState::KEY_PRESSED, boost::bind(&PhysicsPowerApplication::resetScene, this));
	}

	void PhysicsPowerApplication::resetScene() {
		i6e::api::ObjectFacade * of = i6e::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("JengaStick");
		of->deleteAllObjectsOfType("Ball");
		// generate level
		for (uint32_t i = 0; i < 21; i++) { // rows
			const int32_t LENGTH = 15;
			for (int32_t j = 0; j < LENGTH; j++) {
				const int32_t WIDTH = 8;
				for (int32_t k = 0; k < WIDTH; k++) {
					if (i % 2 == 0) {
						i6e::api::objects::GOTemplate tmpl;

						Vec3 pos((k - (WIDTH / 2.0 - 0.5)) * 7.5, 1.25 + 2.5 * i, (j - (LENGTH / 2.0 - 0.5)) * 4.0);
						Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);

						i6e::api::attributeMap paramsPSC;
						pos.insertInMap("pos", paramsPSC);
						rot.insertInMap("rot", paramsPSC);

						tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

						of->createObject("JengaStick", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
					} else {
						i6e::api::objects::GOTemplate tmpl;

						Vec3 pos((j - (LENGTH / 2.0 - 0.5)) * 4.0, 1.25 + 2.5 * i, (k - (WIDTH / 2.0 - 0.5)) * 7.5);
						Quaternion rot(Vec3(1.0, 0.0, 0.0), PI / 2.0);
						rot = rot * Quaternion(Vec3(0.0, 0.0, 1.0), PI / 2.0);

						i6e::api::attributeMap paramsPSC;
						pos.insertInMap("pos", paramsPSC);
						rot.insertInMap("rot", paramsPSC);

						tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

						of->createObject("JengaStick", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false);
					}
				}
			}
		}
	}

} /* namespace sample */
