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

#include "i6engine/api/configs/InputConfig.h"

namespace i6engine {
namespace api {
namespace input {

	Input_Keyboard_Update::Input_Keyboard_Update(const api::KeyState b, const uint32_t c, uint32_t t) : GameMessageStruct(), pressed(b), code(c), text(t) {
	}

	Input_Mouse_Update::Input_Mouse_Update(const int32_t a, const int32_t b) : GameMessageStruct(), intNewX(a), intNewY(b) {
	}

	Input_MouseWheel_Update::Input_MouseWheel_Update(int d) : GameMessageStruct(), diff(d) {
	}

	Input_Button_Update::Input_Button_Update(const bool b, const uint32_t c) : GameMessageStruct(), pressed(b), code(c) {
	}

	Input_Window_Create::Input_Window_Create(void * w) : GameMessageStruct(), window(w) {
	}

	Input_SubscribeKeyEvent_Create::Input_SubscribeKeyEvent_Create(const KeyCode & c, const KeyState t, const boost::function<void(void)> & f) : GameMessageStruct(), code(c), type(t), func(f) {
	}

	Input_SubscribeKeyEvent_Delete::Input_SubscribeKeyEvent_Delete(const KeyCode & c, const KeyState t) : GameMessageStruct(), code(c), type(t) {
	}

	Input_ConsoleRead_Create::Input_ConsoleRead_Create(const std::string & s) : GameMessageStruct(), str(s) {
	}

	Input_Resolution_Update::Input_Resolution_Update(const graphics::Resolution & r) : GameMessageStruct(), resolution(r) {
	}

} /* namespace input */
} /* namespace api */
} /* namespace i6engine */
