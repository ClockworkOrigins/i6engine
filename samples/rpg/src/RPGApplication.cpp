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

#include "RPGApplication.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"

#include "boost/bind.hpp"

namespace sample {

	RPGApplication::RPGApplication() : i6engine::api::Application() {
	}

	RPGApplication::~RPGApplication() {
	}

	void RPGApplication::Initialize() {
	}

	void RPGApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// sets gravity for the game... here like on earth
		i6engine::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(0, -9.81, 0));

		// ambient light for the scene
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(0.0, 0.0, 0.0);

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, "Pressed", boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));

		// loads the RPG demo level
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel("../media/maps/RPGLevel.xml", "Singleplayer");
	}

	void RPGApplication::Tick() {
	}

	bool RPGApplication::ShutdownRequest() {
		return true;
	}

	void RPGApplication::Finalize() {
	}

	void RPGApplication::ShutDown() {
	}

} /* namespace sample */
