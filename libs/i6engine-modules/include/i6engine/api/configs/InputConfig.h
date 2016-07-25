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

/**
 * \addtogroup Input
 * @{
 */

#ifndef __I6ENGINE_API_INPUTCONFIG_H__
#define __I6ENGINE_API_INPUTCONFIG_H__

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/configs/GraphicsConfig.h"

#include "boost/function.hpp"

namespace i6e {
namespace api {
	enum class KeyState : uint16_t;

namespace input {
	enum InputMessageTypes {
		InputReset = 0,
		InputWindow,
		InputSubscribeKeyEvent,
		InputConsoleRead,
		InputResolution,
		InputMessageTypesCount
	};

	/**
	 * \brief notifies state of a key
	 */
	typedef struct ISIXE_MODULES_API Input_Keyboard_Update : GameMessageStruct {
		KeyState pressed;
		KeyCode code;
		uint32_t text;
		Input_Keyboard_Update() {}
		Input_Keyboard_Update(const KeyState b, const KeyCode c, uint32_t t);
		Input_Keyboard_Update * copy() { return new Input_Keyboard_Update(*this); }
	} Input_Keyboard_Update;

	/**
	 * \brief updates mouse position
	 */
	typedef struct ISIXE_MODULES_API Input_Mouse_Update : GameMessageStruct{
		int32_t intNewX;
		int32_t intNewY;
		Input_Mouse_Update() {}
		Input_Mouse_Update(const int32_t a, const int32_t b);
		Input_Mouse_Update * copy() { return new Input_Mouse_Update(*this); }
	} Input_Mouse_Update;

	/**
	 * \brief mouse wheel
	 */
	typedef struct ISIXE_MODULES_API Input_MouseWheel_Update : GameMessageStruct {
		int32_t diff;
		Input_MouseWheel_Update() {
		}
		Input_MouseWheel_Update(int32_t d);
		Input_MouseWheel_Update * copy() {
			return new Input_MouseWheel_Update(*this);
		}
	} Input_MouseWheel_Update;

	/**
	 * \brief registers a RenderWindow for Input subsystem
	 */
	typedef struct Input_Window_Create : GameMessageStruct {
		void * window;
		Input_Window_Create(void * w);
		Input_Window_Create * copy() { return new Input_Window_Create(*this); }
	} Input_Window_Create;

	/**
	 * \brief subscribes a key for one of the key states to call given method if state occures
	 */
	typedef struct Input_SubscribeKeyEvent_Create : GameMessageStruct {
		KeyCode code;
		KeyState type;
		std::function<void(void)> func;
		Input_SubscribeKeyEvent_Create() {}
		Input_SubscribeKeyEvent_Create(const KeyCode & c, const KeyState t, const std::function<void(void)> & f);
		Input_SubscribeKeyEvent_Create * copy() { return new Input_SubscribeKeyEvent_Create(*this); }
	} Input_SubscribeKeyEvent_Create;

	/**
	 * \brief unsubscribes a pair of key and state
	 */
	typedef struct Input_SubscribeKeyEvent_Delete : GameMessageStruct {
		KeyCode code;
		KeyState type;
		Input_SubscribeKeyEvent_Delete() {
		}
		Input_SubscribeKeyEvent_Delete(const KeyCode & c, const KeyState t);
		Input_SubscribeKeyEvent_Delete * copy() {
			return new Input_SubscribeKeyEvent_Delete(*this);
		}
	} Input_SubscribeKeyEvent_Delete;

	/**
	 * \brief read something from stdin
	 */
	typedef struct Input_ConsoleRead_Create : GameMessageStruct {
		std::string str;
		Input_ConsoleRead_Create() {
		}
		Input_ConsoleRead_Create(const std::string & s);
		Input_ConsoleRead_Create * copy() {
			return new Input_ConsoleRead_Create(*this);
		}
	} Input_ConsoleRead_Create;

	/**
	 * \brief updates resolution of render window
	 */
	typedef struct Input_Resolution_Update : GameMessageStruct {
		graphics::Resolution resolution;
		Input_Resolution_Update(const graphics::Resolution & r);
		Input_Resolution_Update * copy() {
			return new Input_Resolution_Update(*this);
		}
	} Input_Resolution_Update;

} /* namespace input */

namespace keyboard {
	enum KeyboardMessageTypes {
		KeyReset = input::InputMessageTypesCount,
		KeyKeyboard,
		KeyboardMessageTypesCount
	};
} /* namespace keyboard */

namespace mouse {
	enum MouseMessageTypes {
		MouReset = keyboard::KeyboardMessageTypesCount,
		MouMouse,
		MouWheel
	};
} /* namespace mouse */

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_INPUTCONFIG_H__ */

/**
 * @}
 */
