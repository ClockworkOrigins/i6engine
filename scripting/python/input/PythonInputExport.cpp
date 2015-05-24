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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace input {

	void setMouseButtonMapping(const i6engine::api::MouseButtonID id, const std::string & strAction) {
		i6engine::api::EngineController::GetSingleton().getInputFacade()->setMouseButtonMapping(id, strAction);
	}

	std::string getMouseButtonMapping(const i6engine::api::MouseButtonID id) {
		return i6engine::api::EngineController::GetSingleton().getInputFacade()->getMouseButtonMapping(id);
	}

	void setKeyMapping(const i6engine::api::KeyCode id, const std::string & strAction) {
		i6engine::api::EngineController::GetSingleton().getInputFacade()->setKeyMapping(id, strAction);
	}

	void removeKeyMapping(const i6engine::api::KeyCode id) {
		i6engine::api::EngineController::GetSingleton().getInputFacade()->removeKeyMapping(id);
	}

	std::string getKeyMapping(const i6engine::api::KeyCode id) {
		return i6engine::api::EngineController::GetSingleton().getInputFacade()->getKeyMapping(id);
	}

	void subscribeKeyEvent(const i6engine::api::KeyCode name, const i6engine::api::KeyState type, const std::string & func) {
		i6engine::api::EngineController::GetSingleton().getInputFacade()->subscribeKeyEvent(name, type, [func]() {
			i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
		});
	}

	void unsubscribeKeyEvent(const i6engine::api::KeyCode name, const i6engine::api::KeyState type) {
		i6engine::api::EngineController::GetSingleton().getInputFacade()->unsubscribeKeyEvent(name, type);
	}

	void resetInputSubSystem() {
		i6engine::api::EngineController::GetSingleton().getInputFacade()->resetSubSystem();
	}

} /* namespace input */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingInputPython) {
	using namespace boost::python;

	enum_<i6engine::api::KeyState>("KeyState")
		.value("KEY_RELEASED", i6engine::api::KeyState::KEY_RELEASED)
		.value("KEY_PRESSED", i6engine::api::KeyState::KEY_PRESSED)
		.value("KEY_HOLD", i6engine::api::KeyState::KEY_HOLD);

	enum_<i6engine::api::KeyCode>("KeyCode")
		.value("KC_UNASSIGNED", i6engine::api::KeyCode::KC_UNASSIGNED)
		.value("KC_ESCAPE", i6engine::api::KeyCode::KC_ESCAPE)
		.value("KC_1", i6engine::api::KeyCode::KC_1)
		.value("KC_2", i6engine::api::KeyCode::KC_2)
		.value("KC_3", i6engine::api::KeyCode::KC_3)
		.value("KC_4", i6engine::api::KeyCode::KC_4)
		.value("KC_5", i6engine::api::KeyCode::KC_5)
		.value("KC_6", i6engine::api::KeyCode::KC_6)
		.value("KC_7", i6engine::api::KeyCode::KC_7)
		.value("KC_8", i6engine::api::KeyCode::KC_8)
		.value("KC_9", i6engine::api::KeyCode::KC_9)
		.value("KC_0", i6engine::api::KeyCode::KC_0)
		.value("KC_MINUS", i6engine::api::KeyCode::KC_MINUS)
		.value("KC_EQUALS", i6engine::api::KeyCode::KC_EQUALS)
		.value("KC_BACK", i6engine::api::KeyCode::KC_BACK)
		.value("KC_TAB", i6engine::api::KeyCode::KC_TAB)
		.value("KC_Q", i6engine::api::KeyCode::KC_Q)
		.value("KC_W", i6engine::api::KeyCode::KC_W)
		.value("KC_E", i6engine::api::KeyCode::KC_E)
		.value("KC_R", i6engine::api::KeyCode::KC_R)
		.value("KC_T", i6engine::api::KeyCode::KC_T)
		.value("KC_Y", i6engine::api::KeyCode::KC_Y)
		.value("KC_U", i6engine::api::KeyCode::KC_U)
		.value("KC_I", i6engine::api::KeyCode::KC_I)
		.value("KC_O", i6engine::api::KeyCode::KC_O)
		.value("KC_P", i6engine::api::KeyCode::KC_P)
		.value("KC_LBRACKET", i6engine::api::KeyCode::KC_LBRACKET)
		.value("KC_RBRACKET", i6engine::api::KeyCode::KC_RBRACKET)
		.value("KC_RETURN", i6engine::api::KeyCode::KC_RETURN)
		.value("KC_LCONTROL", i6engine::api::KeyCode::KC_LCONTROL)
		.value("KC_A", i6engine::api::KeyCode::KC_A)
		.value("KC_S", i6engine::api::KeyCode::KC_S)
		.value("KC_D", i6engine::api::KeyCode::KC_D)
		.value("KC_F", i6engine::api::KeyCode::KC_F)
		.value("KC_G", i6engine::api::KeyCode::KC_G)
		.value("KC_H", i6engine::api::KeyCode::KC_H)
		.value("KC_J", i6engine::api::KeyCode::KC_J)
		.value("KC_K", i6engine::api::KeyCode::KC_K)
		.value("KC_L", i6engine::api::KeyCode::KC_L)
		.value("KC_SEMICOLON", i6engine::api::KeyCode::KC_SEMICOLON)
		.value("KC_APOSTROPHE", i6engine::api::KeyCode::KC_APOSTROPHE)
		.value("KC_GRAVE", i6engine::api::KeyCode::KC_GRAVE)
		.value("KC_LSHIFT", i6engine::api::KeyCode::KC_LSHIFT)
		.value("KC_BACKSLASH", i6engine::api::KeyCode::KC_BACKSLASH)
		.value("KC_Z", i6engine::api::KeyCode::KC_Z)
		.value("KC_X", i6engine::api::KeyCode::KC_X)
		.value("KC_C", i6engine::api::KeyCode::KC_C)
		.value("KC_V", i6engine::api::KeyCode::KC_V)
		.value("KC_B", i6engine::api::KeyCode::KC_B)
		.value("KC_N", i6engine::api::KeyCode::KC_N)
		.value("KC_M", i6engine::api::KeyCode::KC_M)
		.value("KC_COMMA", i6engine::api::KeyCode::KC_COMMA)
		.value("KC_PERIOD", i6engine::api::KeyCode::KC_PERIOD)
		.value("KC_SLASH", i6engine::api::KeyCode::KC_SLASH)
		.value("KC_RSHIFT", i6engine::api::KeyCode::KC_RSHIFT)
		.value("KC_MULTIPLY", i6engine::api::KeyCode::KC_MULTIPLY)
		.value("KC_LMENU", i6engine::api::KeyCode::KC_LMENU)
		.value("KC_SPACE", i6engine::api::KeyCode::KC_SPACE)
		.value("KC_CAPITAL", i6engine::api::KeyCode::KC_CAPITAL)
		.value("KC_F1", i6engine::api::KeyCode::KC_F1)
		.value("KC_F2", i6engine::api::KeyCode::KC_F2)
		.value("KC_F3", i6engine::api::KeyCode::KC_F3)
		.value("KC_F4", i6engine::api::KeyCode::KC_F4)
		.value("KC_F5", i6engine::api::KeyCode::KC_F5)
		.value("KC_F6", i6engine::api::KeyCode::KC_F6)
		.value("KC_F7", i6engine::api::KeyCode::KC_F7)
		.value("KC_F8", i6engine::api::KeyCode::KC_F8)
		.value("KC_F9", i6engine::api::KeyCode::KC_F9)
		.value("KC_F10", i6engine::api::KeyCode::KC_F10)
		.value("KC_NUMLOCK", i6engine::api::KeyCode::KC_NUMLOCK)
		.value("KC_SCROLL", i6engine::api::KeyCode::KC_SCROLL)
		.value("KC_NUMPAD7", i6engine::api::KeyCode::KC_NUMPAD7)
		.value("KC_NUMPAD8", i6engine::api::KeyCode::KC_NUMPAD8)
		.value("KC_NUMPAD9", i6engine::api::KeyCode::KC_NUMPAD9)
		.value("KC_SUBTRACT", i6engine::api::KeyCode::KC_SUBTRACT)
		.value("KC_NUMPAD4", i6engine::api::KeyCode::KC_NUMPAD4)
		.value("KC_NUMPAD5", i6engine::api::KeyCode::KC_NUMPAD5)
		.value("KC_NUMPAD6", i6engine::api::KeyCode::KC_NUMPAD6)
		.value("KC_ADD", i6engine::api::KeyCode::KC_ADD)
		.value("KC_NUMPAD1", i6engine::api::KeyCode::KC_NUMPAD1)
		.value("KC_NUMPAD2", i6engine::api::KeyCode::KC_NUMPAD2)
		.value("KC_NUMPAD3", i6engine::api::KeyCode::KC_NUMPAD3)
		.value("KC_NUMPAD0", i6engine::api::KeyCode::KC_NUMPAD0)
		.value("KC_DECIMAL", i6engine::api::KeyCode::KC_DECIMAL)
		.value("KC_OEM_102", i6engine::api::KeyCode::KC_OEM_102)
		.value("KC_F11", i6engine::api::KeyCode::KC_F11)
		.value("KC_F12", i6engine::api::KeyCode::KC_F12)
		.value("KC_F13", i6engine::api::KeyCode::KC_F13)
		.value("KC_F14", i6engine::api::KeyCode::KC_F14)
		.value("KC_F15", i6engine::api::KeyCode::KC_F15)
		.value("KC_KANA", i6engine::api::KeyCode::KC_KANA)
		.value("KC_ABNT_C1", i6engine::api::KeyCode::KC_ABNT_C1)
		.value("KC_CONVERT", i6engine::api::KeyCode::KC_CONVERT)
		.value("KC_NOCONVERT", i6engine::api::KeyCode::KC_NOCONVERT)
		.value("KC_YEN", i6engine::api::KeyCode::KC_YEN)
		.value("KC_ABNT_C2", i6engine::api::KeyCode::KC_ABNT_C2)
		.value("KC_NUMPADEQUALS", i6engine::api::KeyCode::KC_NUMPADEQUALS)
		.value("KC_PREVTRACK", i6engine::api::KeyCode::KC_PREVTRACK)
		.value("KC_AT", i6engine::api::KeyCode::KC_AT)
		.value("KC_COLON", i6engine::api::KeyCode::KC_COLON)
		.value("KC_UNDERLINE", i6engine::api::KeyCode::KC_UNDERLINE)
		.value("KC_KANJI", i6engine::api::KeyCode::KC_KANJI)
		.value("KC_STOP", i6engine::api::KeyCode::KC_STOP)
		.value("KC_AX", i6engine::api::KeyCode::KC_AX)
		.value("KC_UNLABELED", i6engine::api::KeyCode::KC_UNLABELED)
		.value("KC_NEXTTRACK", i6engine::api::KeyCode::KC_NEXTTRACK)
		.value("KC_NUMPADENTER", i6engine::api::KeyCode::KC_NUMPADENTER)
		.value("KC_RCONTROL", i6engine::api::KeyCode::KC_RCONTROL)
		.value("KC_MUTE", i6engine::api::KeyCode::KC_MUTE)
		.value("KC_CALCULATOR", i6engine::api::KeyCode::KC_CALCULATOR)
		.value("KC_PLAYPAUSE", i6engine::api::KeyCode::KC_PLAYPAUSE)
		.value("KC_MEDIASTOP", i6engine::api::KeyCode::KC_MEDIASTOP)
		.value("KC_VOLUMEDOWN", i6engine::api::KeyCode::KC_VOLUMEDOWN)
		.value("KC_VOLUMEUP", i6engine::api::KeyCode::KC_VOLUMEUP)
		.value("KC_WEBHOME", i6engine::api::KeyCode::KC_WEBHOME)
		.value("KC_NUMPADCOMMA", i6engine::api::KeyCode::KC_NUMPADCOMMA)
		.value("KC_DIVIDE", i6engine::api::KeyCode::KC_DIVIDE)
		.value("KC_SYSRQ", i6engine::api::KeyCode::KC_SYSRQ)
		.value("KC_RMENU", i6engine::api::KeyCode::KC_RMENU)
		.value("KC_PAUSE", i6engine::api::KeyCode::KC_PAUSE)
		.value("KC_HOME", i6engine::api::KeyCode::KC_HOME)
		.value("KC_UP", i6engine::api::KeyCode::KC_UP)
		.value("KC_PGUP", i6engine::api::KeyCode::KC_PGUP)
		.value("KC_LEFT", i6engine::api::KeyCode::KC_LEFT)
		.value("KC_RIGHT", i6engine::api::KeyCode::KC_RIGHT)
		.value("KC_END", i6engine::api::KeyCode::KC_END)
		.value("KC_DOWN", i6engine::api::KeyCode::KC_DOWN)
		.value("KC_PGDOWN", i6engine::api::KeyCode::KC_PGDOWN)
		.value("KC_INSERT", i6engine::api::KeyCode::KC_INSERT)
		.value("KC_DELETE", i6engine::api::KeyCode::KC_DELETE)
		.value("KC_LWIN", i6engine::api::KeyCode::KC_LWIN)
		.value("KC_RWIN", i6engine::api::KeyCode::KC_RWIN)
		.value("KC_APPS", i6engine::api::KeyCode::KC_APPS)
		.value("KC_POWER", i6engine::api::KeyCode::KC_POWER)
		.value("KC_SLEEP", i6engine::api::KeyCode::KC_SLEEP)
		.value("KC_WAKE", i6engine::api::KeyCode::KC_WAKE)
		.value("KC_WEBSEARCH", i6engine::api::KeyCode::KC_WEBSEARCH)
		.value("KC_WEBFAVORITES", i6engine::api::KeyCode::KC_WEBFAVORITES)
		.value("KC_WEBREFRESH", i6engine::api::KeyCode::KC_WEBREFRESH)
		.value("KC_WEBSTOP", i6engine::api::KeyCode::KC_WEBSTOP)
		.value("KC_WEBFORWARD", i6engine::api::KeyCode::KC_WEBFORWARD)
		.value("KC_WEBBACK", i6engine::api::KeyCode::KC_WEBBACK)
		.value("KC_MYCOMPUTER", i6engine::api::KeyCode::KC_MYCOMPUTER)
		.value("KC_MAIL", i6engine::api::KeyCode::KC_MAIL)
		.value("KC_MEDIASELECT", i6engine::api::KeyCode::KC_MEDIASELECT);

	enum_<i6engine::api::MouseButtonID>("MouseButtonID")
		.value("MB_Left", i6engine::api::MouseButtonID::MB_Left)
		.value("MB_Right", i6engine::api::MouseButtonID::MB_Right)
		.value("MB_Middle", i6engine::api::MouseButtonID::MB_Middle)
		.value("MB_Button3", i6engine::api::MouseButtonID::MB_Button3)
		.value("MB_Button4", i6engine::api::MouseButtonID::MB_Button4)
		.value("MB_Button5", i6engine::api::MouseButtonID::MB_Button5)
		.value("MB_Button6", i6engine::api::MouseButtonID::MB_Button6)
		.value("MB_Button7", i6engine::api::MouseButtonID::MB_Button7);

	def("setMouseButtonMapping", &i6engine::python::input::setMouseButtonMapping);
	def("getMouseButtonMapping", &i6engine::python::input::getMouseButtonMapping);
	def("setKeyMapping", &i6engine::python::input::setKeyMapping);
	def("removeKeyMapping", &i6engine::python::input::removeKeyMapping);
	def("getKeyMapping", &i6engine::python::input::getKeyMapping);
	def("subscribeKeyEvent", &i6engine::python::input::subscribeKeyEvent);
	def("unsubscribeKeyEvent", &i6engine::python::input::unsubscribeKeyEvent);
	def("resetInputSubSystem", &i6engine::python::input::resetInputSubSystem);
}
