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

#include "CanvasApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"

#include "boost/bind.hpp"

namespace sample {

	CanvasApplication::CanvasApplication() : i6engine::api::Application(), _percent(0.5) {
	}

	CanvasApplication::~CanvasApplication() {
	}

	void CanvasApplication::Initialize() {
	}

	void CanvasApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->loadCanvas("Canvas", "../media/gui/canvas/CanvasSample.canvas");

		// now all widgets specified in canvas file are loaded and we can access them
		i6engine::api::EngineController::GetSingleton().registerTimer(1000000, [this, gf]() {
			_percent += 0.05;
			if (_percent > 1.0) {
				_percent = 0.0;
			}
			gf->setProgress("Canvas/Background/Bar", _percent);
			return true;
		}, true, 1);

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));
	}

	void CanvasApplication::Tick() {
	}

	bool CanvasApplication::ShutdownRequest() {
		return true;
	}

	void CanvasApplication::Finalize() {
	}

	void CanvasApplication::ShutDown() {
	}

} /* namespace sample */
