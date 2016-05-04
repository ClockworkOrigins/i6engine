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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/InventoryComponent.h"

#include "i6engine/rpg/dialog/DialogManager.h"

#include "i6engine/rpg/quest/QuestLog.h"

#include "boost/bind.hpp"

namespace sample {

	RPGApplication::RPGApplication() : i6e::rpg::RPGApplication(), _showFPS(false) {
	}

	RPGApplication::~RPGApplication() {
	}

	void RPGApplication::AfterInitialize() {
		i6e::rpg::RPGApplication::AfterInitialize();

		// register ESC to close the application
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, []() {
			auto go = *i6e::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player").begin();
			if (!go->getGOC<i6e::rpg::components::InventoryComponent>(i6e::rpg::components::config::ComponentTypes::InventoryComponent)->isActive() && !i6e::rpg::dialog::DialogManager::GetSingleton().isDialogRunning() && !i6e::rpg::quest::QuestLog::GetSingleton().isActive()) {
				i6e::api::EngineController::GetSingletonPtr()->stop();
			}
		});

		// register F12 to take screenshot
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_F12, i6e::api::KeyState::KEY_PRESSED, boost::bind(&i6e::api::GraphicsFacade::takeScreenshot, i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade(), "RPGScreen_", ".jpg"));

		// shows fps (activate/deactive using F1)
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_F1, i6e::api::KeyState::KEY_PRESSED, [this]() {
			if (!_showFPS) {
				i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->showFPS(0.0, 0.0, "RPG/StaticImage", "RPG/Blanko", "RPG", "TbM_Filling");
			} else {
				i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->hideFPS();
			}
			_showFPS = !_showFPS;
		});

		loadLevel("../media/maps/RPGLevel.xml");
	}

} /* namespace sample */
