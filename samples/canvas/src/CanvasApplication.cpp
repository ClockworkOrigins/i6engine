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

#include "CanvasApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"

namespace sample {

	CanvasApplication::CanvasApplication() : i6e::api::Application(), _percent(0.5) {
	}

	CanvasApplication::~CanvasApplication() {
	}

	void CanvasApplication::Initialize() {
	}

	void CanvasApplication::AfterInitialize() {
		i6e::api::GUIFacade * gf = i6eGUIFacade;

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->loadCanvas("Canvas", "../media/gui/canvas/CanvasSample.canvas");

		// now all widgets specified in canvas file are loaded and we can access them
		i6eEngineController->registerTimer(1000000, [this, gf]() {
			_percent += 0.05;
			if (_percent > 1.0) {
				_percent = 0.0;
			}
			gf->setProgress("Canvas/Background/Bar", _percent);
			return true;
		}, true, i6e::core::JobPriorities::Prio_Medium);

		// register ESC to close the application
		i6eInputFacade->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, std::bind(&i6e::api::EngineController::stop, i6eEngineController));
		i6eInputFacade->subscribeKeyEvent(i6e::api::KeyCode::KC_F12, i6e::api::KeyState::KEY_PRESSED, std::bind(&i6e::api::GraphicsFacade::takeScreenshot, i6eGraphicsFacade, "SampleCanvas_", ".png"));
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
