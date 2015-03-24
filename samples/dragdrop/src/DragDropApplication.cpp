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

#include "DragDropApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/bind.hpp"

namespace sample {

	DragDropApplication::DragDropApplication() : i6engine::api::Application() {
	}

	DragDropApplication::~DragDropApplication() {
	}

	void DragDropApplication::Initialize() {
	}

	void DragDropApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->addImage("Background", "RPG/StaticImage", "RPG", "MainMenu", 0.0, 0.0, 1.0, 1.0);

		// 20 slots accepting drops
		for (size_t i = 0; i < 20; i++) {
			gf->addImage("Slot" + std::to_string(i), "RPG/StaticImage", "RPG", "TbM_Filling", 0.1 + 0.1666666 * (i % 5), 0.125 + 0.2 * (i / 5), 0.1333333, 0.175);
			gf->enableDropTarget("Slot" + std::to_string(i), true);
		}

		// 5 images being dragable
		for (size_t i = 0; i < 5; i++) {
			gf->addImage("Image" + std::to_string(i), "RPG/StaticImage", "RPG", "NewCloseButtonNormal", 0.1 + 0.1666666 * (i % 5), 0.125 + 0.2 * (i / 5), 0.1333333, 0.175);
			gf->setDragable("Image" + std::to_string(i), true);
		}

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));
	}

	void DragDropApplication::Tick() {
	}

	bool DragDropApplication::ShutdownRequest() {
		return true;
	}

	void DragDropApplication::Finalize() {
	}

	void DragDropApplication::ShutDown() {
	}

} /* namespace sample */
