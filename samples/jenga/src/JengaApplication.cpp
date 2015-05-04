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
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		// setting shadow technique... currently only additive stencil possible
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6engine::api::graphics::ShadowTechnique::Stencil_Additive);

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6engine::api::objects::GOTemplate tmpl;
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

		inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_RETURN, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&JengaApplication::resetScene, this));
	}

	void JengaApplication::resetScene() {
		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("JengaStick");
		of->deleteAllObjectsOfType("Ball");
		// generate level
		for (uint32_t i = 0; i < 20; i++) { // rows
			for (int32_t j = 0; j < 3; j++) {
				i6engine::api::objects::GOTemplate tmpl;

				Vec3 pos(0.0, 0.5 + 1.0 * i, (j - 1) * 2.5);
				Quaternion rot;

				if (i % 2 == 1) {
					pos.setZ(0.0);
					pos.setX((j - 1) * 2.5);
					rot = Quaternion(Vec3(0.0, 1.0, 0.0), PI / 2.0);
				}

				i6engine::api::attributeMap paramsPSC;
				pos.insertInMap("pos", paramsPSC);
				rot.insertInMap("rot", paramsPSC);

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

				if (j == 1) {
					i6engine::api::attributeMap paramsMesh;
					paramsMesh["material"] = "JengaStick_Middle";

					tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("MeshAppearance", paramsMesh, "", false, false));
				}

				of->createObject("JengaStick", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
			}
		}
	}

} /* namespace sample */
