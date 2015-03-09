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

namespace i6engine {
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
	typedef struct Input_Keyboard_Update : GameMessageStruct {
		api::KeyState pressed;
		uint32_t code;
		uint32_t text;
		Input_Keyboard_Update() {}
		Input_Keyboard_Update(const api::KeyState b, const uint32_t c, uint32_t t);
		Input_Keyboard_Update * copy() { return new Input_Keyboard_Update(*this); }
	} Input_Keyboard_Update;

	/**
	 * \brief updates mouse position
	 */
	typedef struct Input_Mouse_Update : GameMessageStruct {
		int32_t intNewX;
		int32_t intNewY;
		Input_Mouse_Update() {}
		Input_Mouse_Update(const int32_t a, const int32_t b);
		Input_Mouse_Update * copy() { return new Input_Mouse_Update(*this); }
	} Input_Mouse_Update;

	/**
	 * \brief mouse wheel
	 */
	typedef struct Input_MouseWheel_Update : GameMessageStruct {
		int32_t diff;
		Input_MouseWheel_Update() {
		}
		Input_MouseWheel_Update(int32_t d);
		Input_MouseWheel_Update * copy() {
			return new Input_MouseWheel_Update(*this);
		}
	} Input_MouseWheel_Update;

	/**
	 * \brief notifies state of a mouse button
	 */
	typedef struct Input_Button_Update : GameMessageStruct {
		bool pressed;
		uint32_t code;
		Input_Button_Update() {}
		Input_Button_Update(const bool b, const uint32_t c);
		Input_Button_Update * copy() { return new Input_Button_Update(*this); }
	} Input_Button_Update;

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
		std::string type;
		boost::function<void(void)> func;
		Input_SubscribeKeyEvent_Create() {}
		Input_SubscribeKeyEvent_Create(const KeyCode & c, const std::string & t, const boost::function<void(void)> & f);
		Input_SubscribeKeyEvent_Create * copy() { return new Input_SubscribeKeyEvent_Create(*this); }
	} Input_SubscribeKeyEvent_Create;

	/**
	 * \brief unsubscribes a pair of key and state
	 */
	typedef struct Input_SubscribeKeyEvent_Delete : GameMessageStruct {
		KeyCode code;
		std::string type;
		Input_SubscribeKeyEvent_Delete() {
		}
		Input_SubscribeKeyEvent_Delete(const KeyCode & c, const std::string & t);
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
		MouButton,
		MouWheel
	};
} /* namespace mouse */

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_INPUTCONFIG_H__ */

/**
 * @}
 */
