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

#include "TooltipApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

namespace sample {

	TooltipApplication::TooltipApplication() : i6e::api::Application(), _progressTimer(), _counter(0) {
	}

	TooltipApplication::~TooltipApplication() {
	}

	void TooltipApplication::Initialize() {
	}

	void TooltipApplication::AfterInitialize() {
		i6e::api::GUIFacade * gf = i6e::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->addImage("Background", "RPG/StaticImage", "RPG", "MainMenu", 0.0, 0.0, 1.0, 1.0);

		// a simple image
		gf->addImage("Image", "RPG/StaticImage", "RPG", "NewCloseButtonNormal", 0.1, 0.1, 0.2, 0.25);
		gf->setTooltip("Image", "To add a tooltip is really simple and just a one liner.");
		gf->playAnimation("Image", "HideAndShow");

		// an editbox changing the text of the print
		gf->addEditbox("Editbox", "RPG/Editbox", 0.1, 0.4, 0.4, 0.05, "Enter some text", [gf](std::string s) {
			gf->setText("Print", s);
		});
		gf->setTooltip("Editbox", "In an Editbox you can enter some texts.~In this example the text is added to the print in the right upper corner.");

		// a print
		gf->addPrint("Print", "RPG/Blanko", 0.5, 0.1, "Empty", i6e::api::gui::Alignment::Left, -1);
		gf->setSize("Print", 0.4, 0.05);
		gf->setTooltip("Print", "The text in this line is influenced by the Editbox.~Just type in there what you like to see here.");

		// a toggle button
		gf->addToggleButton("ToggleButton", "RPG/ToggleButton", 0.6, 0.4, 0.02, 0.02, false, [this, gf](bool b) {
			if (b) {
				gf->setTooltip("ToggleButton", "Press to deactivate progress bar.");
				_progressTimer = i6e::api::EngineController::GetSingleton().registerTimer(1000000, [this, gf]() {
					_counter = (_counter + 1) % 11;
					gf->setProgress("ProgressBar", _counter / 10.0);
					return true;
				}, true, i6e::core::JobPriorities::Prio_Medium);
				gf->pauseAnimation("ToggleButton", "RotateY");
			} else {
				gf->setTooltip("ToggleButton", "Press to activate progress bar.");
				i6e::api::EngineController::GetSingleton().removeTimerID(_progressTimer);
				gf->unpauseAnimation("ToggleButton", "RotateY");
			}
		});
		gf->setTooltip("ToggleButton", "Press to activate progress bar.");
		gf->playAnimation("ToggleButton", "RotateY");

		// a progress bar adding 10 percent every second
		gf->addProgressBar("ProgressBar", "RPG/ProgressBar", 0.7, 0.4, 0.2, 0.05);
		gf->setTooltip("ProgressBar", "A progress bar is used for showing progress or for healthbars.");

		// register ESC to close the application
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, std::bind(&i6e::api::EngineController::stop, i6e::api::EngineController::GetSingletonPtr()));
	}

	void TooltipApplication::Tick() {
	}

	bool TooltipApplication::ShutdownRequest() {
		return true;
	}

	void TooltipApplication::Finalize() {
	}

	void TooltipApplication::ShutDown() {
	}

} /* namespace sample */
