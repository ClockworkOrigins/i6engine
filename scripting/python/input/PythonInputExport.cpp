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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace input {

	void setKeyMapping(const i6e::api::KeyCode id, const std::string & strAction) {
		i6eInputFacade->setKeyMapping(id, strAction);
	}

	void removeKeyMapping(const i6e::api::KeyCode id) {
		i6eInputFacade->removeKeyMapping(id);
	}

	std::string getKeyMapping(const i6e::api::KeyCode id) {
		return i6eInputFacade->getKeyMapping(id);
	}

	void subscribeKeyEvent(const i6e::api::KeyCode name, const i6e::api::KeyState type, const std::string & func) {
		i6eInputFacade->subscribeKeyEvent(name, type, [func]() {
			i6eScriptingFacade->callFunction<void>(func);
		});
	}

	void unsubscribeKeyEvent(const i6e::api::KeyCode name, const i6e::api::KeyState type) {
		i6eInputFacade->unsubscribeKeyEvent(name, type);
	}

	void resetInputSubSystem() {
		i6eInputFacade->resetSubSystem();
	}

} /* namespace input */
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingInputPython) {
	using namespace boost::python;

	enum_<i6e::api::KeyState>("KeyState")
		.value("KEY_RELEASED", i6e::api::KeyState::KEY_RELEASED)
		.value("KEY_PRESSED", i6e::api::KeyState::KEY_PRESSED)
		.value("KEY_HOLD", i6e::api::KeyState::KEY_HOLD)
		.export_values();

	enum_<i6e::api::KeyCode>("KeyCode")
		.value("KC_UNASSIGNED", i6e::api::KeyCode::KC_UNASSIGNED)
		.value("KC_ESCAPE", i6e::api::KeyCode::KC_ESCAPE)
		.value("KC_1", i6e::api::KeyCode::KC_1)
		.value("KC_2", i6e::api::KeyCode::KC_2)
		.value("KC_3", i6e::api::KeyCode::KC_3)
		.value("KC_4", i6e::api::KeyCode::KC_4)
		.value("KC_5", i6e::api::KeyCode::KC_5)
		.value("KC_6", i6e::api::KeyCode::KC_6)
		.value("KC_7", i6e::api::KeyCode::KC_7)
		.value("KC_8", i6e::api::KeyCode::KC_8)
		.value("KC_9", i6e::api::KeyCode::KC_9)
		.value("KC_0", i6e::api::KeyCode::KC_0)
		.value("KC_MINUS", i6e::api::KeyCode::KC_MINUS)
		.value("KC_EQUALS", i6e::api::KeyCode::KC_EQUALS)
		.value("KC_BACK", i6e::api::KeyCode::KC_BACK)
		.value("KC_TAB", i6e::api::KeyCode::KC_TAB)
		.value("KC_Q", i6e::api::KeyCode::KC_Q)
		.value("KC_W", i6e::api::KeyCode::KC_W)
		.value("KC_E", i6e::api::KeyCode::KC_E)
		.value("KC_R", i6e::api::KeyCode::KC_R)
		.value("KC_T", i6e::api::KeyCode::KC_T)
		.value("KC_Y", i6e::api::KeyCode::KC_Y)
		.value("KC_U", i6e::api::KeyCode::KC_U)
		.value("KC_I", i6e::api::KeyCode::KC_I)
		.value("KC_O", i6e::api::KeyCode::KC_O)
		.value("KC_P", i6e::api::KeyCode::KC_P)
		.value("KC_LBRACKET", i6e::api::KeyCode::KC_LBRACKET)
		.value("KC_RBRACKET", i6e::api::KeyCode::KC_RBRACKET)
		.value("KC_RETURN", i6e::api::KeyCode::KC_RETURN)
		.value("KC_LCONTROL", i6e::api::KeyCode::KC_LCONTROL)
		.value("KC_A", i6e::api::KeyCode::KC_A)
		.value("KC_S", i6e::api::KeyCode::KC_S)
		.value("KC_D", i6e::api::KeyCode::KC_D)
		.value("KC_F", i6e::api::KeyCode::KC_F)
		.value("KC_G", i6e::api::KeyCode::KC_G)
		.value("KC_H", i6e::api::KeyCode::KC_H)
		.value("KC_J", i6e::api::KeyCode::KC_J)
		.value("KC_K", i6e::api::KeyCode::KC_K)
		.value("KC_L", i6e::api::KeyCode::KC_L)
		.value("KC_SEMICOLON", i6e::api::KeyCode::KC_SEMICOLON)
		.value("KC_APOSTROPHE", i6e::api::KeyCode::KC_APOSTROPHE)
		.value("KC_GRAVE", i6e::api::KeyCode::KC_GRAVE)
		.value("KC_LSHIFT", i6e::api::KeyCode::KC_LSHIFT)
		.value("KC_BACKSLASH", i6e::api::KeyCode::KC_BACKSLASH)
		.value("KC_Z", i6e::api::KeyCode::KC_Z)
		.value("KC_X", i6e::api::KeyCode::KC_X)
		.value("KC_C", i6e::api::KeyCode::KC_C)
		.value("KC_V", i6e::api::KeyCode::KC_V)
		.value("KC_B", i6e::api::KeyCode::KC_B)
		.value("KC_N", i6e::api::KeyCode::KC_N)
		.value("KC_M", i6e::api::KeyCode::KC_M)
		.value("KC_COMMA", i6e::api::KeyCode::KC_COMMA)
		.value("KC_PERIOD", i6e::api::KeyCode::KC_PERIOD)
		.value("KC_SLASH", i6e::api::KeyCode::KC_SLASH)
		.value("KC_RSHIFT", i6e::api::KeyCode::KC_RSHIFT)
		.value("KC_MULTIPLY", i6e::api::KeyCode::KC_MULTIPLY)
		.value("KC_LMENU", i6e::api::KeyCode::KC_LMENU)
		.value("KC_SPACE", i6e::api::KeyCode::KC_SPACE)
		.value("KC_CAPITAL", i6e::api::KeyCode::KC_CAPITAL)
		.value("KC_F1", i6e::api::KeyCode::KC_F1)
		.value("KC_F2", i6e::api::KeyCode::KC_F2)
		.value("KC_F3", i6e::api::KeyCode::KC_F3)
		.value("KC_F4", i6e::api::KeyCode::KC_F4)
		.value("KC_F5", i6e::api::KeyCode::KC_F5)
		.value("KC_F6", i6e::api::KeyCode::KC_F6)
		.value("KC_F7", i6e::api::KeyCode::KC_F7)
		.value("KC_F8", i6e::api::KeyCode::KC_F8)
		.value("KC_F9", i6e::api::KeyCode::KC_F9)
		.value("KC_F10", i6e::api::KeyCode::KC_F10)
		.value("KC_NUMLOCK", i6e::api::KeyCode::KC_NUMLOCK)
		.value("KC_SCROLL", i6e::api::KeyCode::KC_SCROLL)
		.value("KC_NUMPAD7", i6e::api::KeyCode::KC_NUMPAD7)
		.value("KC_NUMPAD8", i6e::api::KeyCode::KC_NUMPAD8)
		.value("KC_NUMPAD9", i6e::api::KeyCode::KC_NUMPAD9)
		.value("KC_SUBTRACT", i6e::api::KeyCode::KC_SUBTRACT)
		.value("KC_NUMPAD4", i6e::api::KeyCode::KC_NUMPAD4)
		.value("KC_NUMPAD5", i6e::api::KeyCode::KC_NUMPAD5)
		.value("KC_NUMPAD6", i6e::api::KeyCode::KC_NUMPAD6)
		.value("KC_ADD", i6e::api::KeyCode::KC_ADD)
		.value("KC_NUMPAD1", i6e::api::KeyCode::KC_NUMPAD1)
		.value("KC_NUMPAD2", i6e::api::KeyCode::KC_NUMPAD2)
		.value("KC_NUMPAD3", i6e::api::KeyCode::KC_NUMPAD3)
		.value("KC_NUMPAD0", i6e::api::KeyCode::KC_NUMPAD0)
		.value("KC_DECIMAL", i6e::api::KeyCode::KC_DECIMAL)
		.value("KC_OEM_102", i6e::api::KeyCode::KC_OEM_102)
		.value("KC_F11", i6e::api::KeyCode::KC_F11)
		.value("KC_F12", i6e::api::KeyCode::KC_F12)
		.value("KC_F13", i6e::api::KeyCode::KC_F13)
		.value("KC_F14", i6e::api::KeyCode::KC_F14)
		.value("KC_F15", i6e::api::KeyCode::KC_F15)
		.value("KC_KANA", i6e::api::KeyCode::KC_KANA)
		.value("KC_ABNT_C1", i6e::api::KeyCode::KC_ABNT_C1)
		.value("KC_CONVERT", i6e::api::KeyCode::KC_CONVERT)
		.value("KC_NOCONVERT", i6e::api::KeyCode::KC_NOCONVERT)
		.value("KC_YEN", i6e::api::KeyCode::KC_YEN)
		.value("KC_ABNT_C2", i6e::api::KeyCode::KC_ABNT_C2)
		.value("KC_NUMPADEQUALS", i6e::api::KeyCode::KC_NUMPADEQUALS)
		.value("KC_PREVTRACK", i6e::api::KeyCode::KC_PREVTRACK)
		.value("KC_AT", i6e::api::KeyCode::KC_AT)
		.value("KC_COLON", i6e::api::KeyCode::KC_COLON)
		.value("KC_UNDERLINE", i6e::api::KeyCode::KC_UNDERLINE)
		.value("KC_KANJI", i6e::api::KeyCode::KC_KANJI)
		.value("KC_STOP", i6e::api::KeyCode::KC_STOP)
		.value("KC_AX", i6e::api::KeyCode::KC_AX)
		.value("KC_UNLABELED", i6e::api::KeyCode::KC_UNLABELED)
		.value("KC_NEXTTRACK", i6e::api::KeyCode::KC_NEXTTRACK)
		.value("KC_NUMPADENTER", i6e::api::KeyCode::KC_NUMPADENTER)
		.value("KC_RCONTROL", i6e::api::KeyCode::KC_RCONTROL)
		.value("KC_MUTE", i6e::api::KeyCode::KC_MUTE)
		.value("KC_CALCULATOR", i6e::api::KeyCode::KC_CALCULATOR)
		.value("KC_PLAYPAUSE", i6e::api::KeyCode::KC_PLAYPAUSE)
		.value("KC_MEDIASTOP", i6e::api::KeyCode::KC_MEDIASTOP)
		.value("KC_VOLUMEDOWN", i6e::api::KeyCode::KC_VOLUMEDOWN)
		.value("KC_VOLUMEUP", i6e::api::KeyCode::KC_VOLUMEUP)
		.value("KC_WEBHOME", i6e::api::KeyCode::KC_WEBHOME)
		.value("KC_NUMPADCOMMA", i6e::api::KeyCode::KC_NUMPADCOMMA)
		.value("KC_DIVIDE", i6e::api::KeyCode::KC_DIVIDE)
		.value("KC_SYSRQ", i6e::api::KeyCode::KC_SYSRQ)
		.value("KC_RMENU", i6e::api::KeyCode::KC_RMENU)
		.value("KC_PAUSE", i6e::api::KeyCode::KC_PAUSE)
		.value("KC_HOME", i6e::api::KeyCode::KC_HOME)
		.value("KC_UP", i6e::api::KeyCode::KC_UP)
		.value("KC_PGUP", i6e::api::KeyCode::KC_PGUP)
		.value("KC_LEFT", i6e::api::KeyCode::KC_LEFT)
		.value("KC_RIGHT", i6e::api::KeyCode::KC_RIGHT)
		.value("KC_END", i6e::api::KeyCode::KC_END)
		.value("KC_DOWN", i6e::api::KeyCode::KC_DOWN)
		.value("KC_PGDOWN", i6e::api::KeyCode::KC_PGDOWN)
		.value("KC_INSERT", i6e::api::KeyCode::KC_INSERT)
		.value("KC_DELETE", i6e::api::KeyCode::KC_DELETE)
		.value("KC_LWIN", i6e::api::KeyCode::KC_LWIN)
		.value("KC_RWIN", i6e::api::KeyCode::KC_RWIN)
		.value("KC_APPS", i6e::api::KeyCode::KC_APPS)
		.value("KC_POWER", i6e::api::KeyCode::KC_POWER)
		.value("KC_SLEEP", i6e::api::KeyCode::KC_SLEEP)
		.value("KC_WAKE", i6e::api::KeyCode::KC_WAKE)
		.value("KC_WEBSEARCH", i6e::api::KeyCode::KC_WEBSEARCH)
		.value("KC_WEBFAVORITES", i6e::api::KeyCode::KC_WEBFAVORITES)
		.value("KC_WEBREFRESH", i6e::api::KeyCode::KC_WEBREFRESH)
		.value("KC_WEBSTOP", i6e::api::KeyCode::KC_WEBSTOP)
		.value("KC_WEBFORWARD", i6e::api::KeyCode::KC_WEBFORWARD)
		.value("KC_WEBBACK", i6e::api::KeyCode::KC_WEBBACK)
		.value("KC_MYCOMPUTER", i6e::api::KeyCode::KC_MYCOMPUTER)
		.value("KC_MAIL", i6e::api::KeyCode::KC_MAIL)
		.value("KC_MEDIASELECT", i6e::api::KeyCode::KC_MEDIASELECT)
		.value("KC_MBLeft", i6e::api::KeyCode::KC_MBLeft)
		.value("KC_MBRight", i6e::api::KeyCode::KC_MBRight)
		.value("KC_MBMiddle", i6e::api::KeyCode::KC_MBMiddle)
		.value("KC_MBButton3", i6e::api::KeyCode::KC_MBButton3)
		.value("KC_MBButton4", i6e::api::KeyCode::KC_MBButton4)
		.value("KC_MBButton5", i6e::api::KeyCode::KC_MBButton5)
		.value("KC_MBButton6", i6e::api::KeyCode::KC_MBButton6)
		.value("KC_MBButton7", i6e::api::KeyCode::KC_MBButton7)
		.export_values();

	def("setKeyMapping", &i6e::python::input::setKeyMapping);
	def("removeKeyMapping", &i6e::python::input::removeKeyMapping);
	def("getKeyMapping", &i6e::python::input::getKeyMapping);
	def("subscribeKeyEvent", &i6e::python::input::subscribeKeyEvent);
	def("unsubscribeKeyEvent", &i6e::python::input::unsubscribeKeyEvent);
	def("resetInputSubSystem", &i6e::python::input::resetInputSubSystem);
}
