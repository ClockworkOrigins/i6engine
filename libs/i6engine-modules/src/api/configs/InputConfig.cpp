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

#include "i6engine/api/configs/InputConfig.h"

namespace i6e {
namespace api {
namespace input {

	Input_Keyboard_Update::Input_Keyboard_Update(const KeyState b, const KeyCode c, uint32_t t) : GameMessageStruct(), pressed(b), code(c), text(t) {
	}

	Input_Mouse_Update::Input_Mouse_Update(const int32_t a, const int32_t b) : GameMessageStruct(), intNewX(a), intNewY(b) {
	}

	Input_MouseWheel_Update::Input_MouseWheel_Update(int d) : GameMessageStruct(), diff(d) {
	}

	Input_Window_Create::Input_Window_Create(void * w) : GameMessageStruct(), window(w) {
	}

	Input_SubscribeKeyEvent_Create::Input_SubscribeKeyEvent_Create(const KeyCode & c, const KeyState t, const std::function<void(void)> & f) : GameMessageStruct(), code(c), type(t), func(f) {
	}

	Input_SubscribeKeyEvent_Delete::Input_SubscribeKeyEvent_Delete(const KeyCode & c, const KeyState t) : GameMessageStruct(), code(c), type(t) {
	}

	Input_ConsoleRead_Create::Input_ConsoleRead_Create(const std::string & s) : GameMessageStruct(), str(s) {
	}

	Input_Resolution_Update::Input_Resolution_Update(const graphics::Resolution & r) : GameMessageStruct(), resolution(r) {
	}

} /* namespace input */
} /* namespace api */
} /* namespace i6e */
