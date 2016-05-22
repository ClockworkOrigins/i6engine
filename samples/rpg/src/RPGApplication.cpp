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
