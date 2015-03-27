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

#include "TooltipApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/bind.hpp"

namespace sample {

	TooltipApplication::TooltipApplication() : i6engine::api::Application(), _progressTimer(), _counter(0) {
	}

	TooltipApplication::~TooltipApplication() {
	}

	void TooltipApplication::Initialize() {
	}

	void TooltipApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->addImage("Background", "RPG/StaticImage", "RPG", "MainMenu", 0.0, 0.0, 1.0, 1.0);

		// a simple image
		gf->addImage("Image", "RPG/StaticImage", "RPG", "NewCloseButtonNormal", 0.1, 0.1, 0.2, 0.25);
		gf->setTooltip("Image", "To add a tooltip is really simple and just a one liner.");

		// an editbox changing the text of the print
		gf->addEditbox("Editbox", "RPG/Editbox", 0.1, 0.4, 0.4, 0.05, "Enter some text", [gf](std::string s) {
			gf->setText("Print", s);
		});
		gf->setTooltip("Editbox", "In an Editbox you can enter some texts.~In this example the text is added to the print in the right upper corner.");

		// a print
		gf->addPrint("Print", "RPG/Blanko", 0.5, 0.1, "Empty", i6engine::api::gui::Alignment::Left, -1);
		gf->setSize("Print", 0.4, 0.05);
		gf->setTooltip("Print", "The text in this line is influenced by the Editbox.~Just type in there what you like to see here.");

		// a toggle button
		gf->addToggleButton("ToggleButton", "RPG/ToggleButton", 0.6, 0.4, 0.02, 0.02, false, [this, gf](bool b) {
			if (b) {
				gf->setTooltip("ToggleButton", "Press to deactivate progress bar.");
				_progressTimer = i6engine::api::EngineController::GetSingleton().registerTimer(1000000, [this, gf]() {
					_counter = (_counter + 1) % 11;
					gf->setProgress("ProgressBar", _counter / 10.0);
					return true;
				}, true, 1);
			} else {
				gf->setTooltip("ToggleButton", "Press to activate progress bar.");
				i6engine::api::EngineController::GetSingleton().removeTimerID(_progressTimer);
			}
		});
		gf->setTooltip("ToggleButton", "Press to activate progress bar.");

		// a progress bar adding 10 percent every second
		gf->addProgressBar("ProgressBar", "RPG/ProgressBar", 0.7, 0.4, 0.2, 0.05);
		gf->setTooltip("ProgressBar", "A progress bar is used for showing progress or for healthbars.");

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));
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
