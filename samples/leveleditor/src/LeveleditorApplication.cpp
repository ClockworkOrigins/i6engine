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

#include "LeveleditorApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/InputFacade.h"

#include "boost/bind.hpp"

namespace sample {

	LeveleditorApplication::LeveleditorApplication() : i6engine::editor::Editor("Leveleditor Sample") {
	}

	LeveleditorApplication::~LeveleditorApplication() {
	}

	void LeveleditorApplication::AfterInitialize() {
		Editor::AfterInitialize();

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));
	}

} /* namespace sample */
