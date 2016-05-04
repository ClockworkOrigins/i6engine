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

#include "LuaPhysicsExport.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

namespace i6e {
namespace lua {
namespace input {

	void setKeyMapping(const i6e::api::KeyCode id, const std::string & strAction) {
		i6e::api::EngineController::GetSingleton().getInputFacade()->setKeyMapping(id, strAction);
	}

	void removeKeyMapping(const i6e::api::KeyCode id) {
		i6e::api::EngineController::GetSingleton().getInputFacade()->removeKeyMapping(id);
	}

	std::string getKeyMapping(const i6e::api::KeyCode id) {
		return i6e::api::EngineController::GetSingleton().getInputFacade()->getKeyMapping(id);
	}

	void subscribeKeyEvent(const i6e::api::KeyCode name, const i6e::api::KeyState type, const std::string & func) {
		i6e::api::EngineController::GetSingleton().getInputFacade()->subscribeKeyEvent(name, type, [func]() {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func);
		});
	}

	void unsubscribeKeyEvent(const i6e::api::KeyCode name, const i6e::api::KeyState type) {
		i6e::api::EngineController::GetSingleton().getInputFacade()->unsubscribeKeyEvent(name, type);
	}

	void resetInputSubSystem() {
		i6e::api::EngineController::GetSingleton().getInputFacade()->resetSubSystem();
	}

} /* namespace input */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerInput() {
	return
		class_<i6e::api::KeyState>("KeyState")
			.def(constructor<>())
			.enum_("KeyState")
			[
				value("KEY_RELEASED", int(i6e::api::KeyState::KEY_RELEASED)),
				value("KEY_PRESSED", int(i6e::api::KeyState::KEY_PRESSED)),
				value("KEY_HOLD", int(i6e::api::KeyState::KEY_HOLD))
			],

		class_<i6e::api::KeyCode>("KeyCode")
			.def(constructor<>())
			.enum_("KeyCode")
			[
				value("KC_UNASSIGNED", int(i6e::api::KeyCode::KC_UNASSIGNED)),
				value("KC_ESCAPE", int(i6e::api::KeyCode::KC_ESCAPE)),
				value("KC_1", int(i6e::api::KeyCode::KC_1)),
				value("KC_2", int(i6e::api::KeyCode::KC_2)),
				value("KC_3", int(i6e::api::KeyCode::KC_3)),
				value("KC_4", int(i6e::api::KeyCode::KC_4)),
				value("KC_5", int(i6e::api::KeyCode::KC_5)),
				value("KC_6", int(i6e::api::KeyCode::KC_6)),
				value("KC_7", int(i6e::api::KeyCode::KC_7)),
				value("KC_8", int(i6e::api::KeyCode::KC_8)),
				value("KC_9", int(i6e::api::KeyCode::KC_9)),
				value("KC_0", int(i6e::api::KeyCode::KC_0)),
				value("KC_MINUS", int(i6e::api::KeyCode::KC_MINUS)),
				value("KC_EQUALS", int(i6e::api::KeyCode::KC_EQUALS)),
				value("KC_BACK", int(i6e::api::KeyCode::KC_BACK)),
				value("KC_TAB", int(i6e::api::KeyCode::KC_TAB)),
				value("KC_Q", int(i6e::api::KeyCode::KC_Q)),
				value("KC_W", int(i6e::api::KeyCode::KC_W)),
				value("KC_E", int(i6e::api::KeyCode::KC_E)),
				value("KC_R", int(i6e::api::KeyCode::KC_R)),
				value("KC_T", int(i6e::api::KeyCode::KC_T)),
				value("KC_Y", int(i6e::api::KeyCode::KC_Y)),
				value("KC_U", int(i6e::api::KeyCode::KC_U)),
				value("KC_I", int(i6e::api::KeyCode::KC_I)),
				value("KC_O", int(i6e::api::KeyCode::KC_O)),
				value("KC_P", int(i6e::api::KeyCode::KC_P)),
				value("KC_LBRACKET", int(i6e::api::KeyCode::KC_LBRACKET)),
				value("KC_RBRACKET", int(i6e::api::KeyCode::KC_RBRACKET)),
				value("KC_RETURN", int(i6e::api::KeyCode::KC_RETURN)),
				value("KC_LCONTROL", int(i6e::api::KeyCode::KC_LCONTROL)),
				value("KC_A", int(i6e::api::KeyCode::KC_A)),
				value("KC_S", int(i6e::api::KeyCode::KC_S)),
				value("KC_D", int(i6e::api::KeyCode::KC_D)),
				value("KC_F", int(i6e::api::KeyCode::KC_F)),
				value("KC_G", int(i6e::api::KeyCode::KC_G)),
				value("KC_H", int(i6e::api::KeyCode::KC_H)),
				value("KC_J", int(i6e::api::KeyCode::KC_J)),
				value("KC_K", int(i6e::api::KeyCode::KC_K)),
				value("KC_L", int(i6e::api::KeyCode::KC_L)),
				value("KC_SEMICOLON", int(i6e::api::KeyCode::KC_SEMICOLON)),
				value("KC_APOSTROPHE", int(i6e::api::KeyCode::KC_APOSTROPHE)),
				value("KC_GRAVE", int(i6e::api::KeyCode::KC_GRAVE)),
				value("KC_LSHIFT", int(i6e::api::KeyCode::KC_LSHIFT)),
				value("KC_BACKSLASH", int(i6e::api::KeyCode::KC_BACKSLASH)),
				value("KC_Z", int(i6e::api::KeyCode::KC_Z)),
				value("KC_X", int(i6e::api::KeyCode::KC_X)),
				value("KC_C", int(i6e::api::KeyCode::KC_C)),
				value("KC_V", int(i6e::api::KeyCode::KC_V)),
				value("KC_B", int(i6e::api::KeyCode::KC_B)),
				value("KC_N", int(i6e::api::KeyCode::KC_N)),
				value("KC_M", int(i6e::api::KeyCode::KC_M)),
				value("KC_COMMA", int(i6e::api::KeyCode::KC_COMMA)),
				value("KC_PERIOD", int(i6e::api::KeyCode::KC_PERIOD)),
				value("KC_SLASH", int(i6e::api::KeyCode::KC_SLASH)),
				value("KC_RSHIFT", int(i6e::api::KeyCode::KC_RSHIFT)),
				value("KC_MULTIPLY", int(i6e::api::KeyCode::KC_MULTIPLY)),
				value("KC_LMENU", int(i6e::api::KeyCode::KC_LMENU)),
				value("KC_SPACE", int(i6e::api::KeyCode::KC_SPACE)),
				value("KC_CAPITAL", int(i6e::api::KeyCode::KC_CAPITAL)),
				value("KC_F1", int(i6e::api::KeyCode::KC_F1)),
				value("KC_F2", int(i6e::api::KeyCode::KC_F2)),
				value("KC_F3", int(i6e::api::KeyCode::KC_F3)),
				value("KC_F4", int(i6e::api::KeyCode::KC_F4)),
				value("KC_F5", int(i6e::api::KeyCode::KC_F5)),
				value("KC_F6", int(i6e::api::KeyCode::KC_F6)),
				value("KC_F7", int(i6e::api::KeyCode::KC_F7)),
				value("KC_F8", int(i6e::api::KeyCode::KC_F8)),
				value("KC_F9", int(i6e::api::KeyCode::KC_F9)),
				value("KC_F10", int(i6e::api::KeyCode::KC_F10)),
				value("KC_NUMLOCK", int(i6e::api::KeyCode::KC_NUMLOCK)),
				value("KC_SCROLL", int(i6e::api::KeyCode::KC_SCROLL)),
				value("KC_NUMPAD7", int(i6e::api::KeyCode::KC_NUMPAD7)),
				value("KC_NUMPAD8", int(i6e::api::KeyCode::KC_NUMPAD8)),
				value("KC_NUMPAD9", int(i6e::api::KeyCode::KC_NUMPAD9)),
				value("KC_SUBTRACT", int(i6e::api::KeyCode::KC_SUBTRACT)),
				value("KC_NUMPAD4", int(i6e::api::KeyCode::KC_NUMPAD4)),
				value("KC_NUMPAD5", int(i6e::api::KeyCode::KC_NUMPAD5)),
				value("KC_NUMPAD6", int(i6e::api::KeyCode::KC_NUMPAD6)),
				value("KC_ADD", int(i6e::api::KeyCode::KC_ADD)),
				value("KC_NUMPAD1", int(i6e::api::KeyCode::KC_NUMPAD1)),
				value("KC_NUMPAD2", int(i6e::api::KeyCode::KC_NUMPAD2)),
				value("KC_NUMPAD3", int(i6e::api::KeyCode::KC_NUMPAD3)),
				value("KC_NUMPAD0", int(i6e::api::KeyCode::KC_NUMPAD0)),
				value("KC_DECIMAL", int(i6e::api::KeyCode::KC_DECIMAL)),
				value("KC_OEM_102", int(i6e::api::KeyCode::KC_OEM_102)),
				value("KC_F11", int(i6e::api::KeyCode::KC_F11)),
				value("KC_F12", int(i6e::api::KeyCode::KC_F12)),
				value("KC_F13", int(i6e::api::KeyCode::KC_F13)),
				value("KC_F14", int(i6e::api::KeyCode::KC_F14)),
				value("KC_F15", int(i6e::api::KeyCode::KC_F15)),
				value("KC_KANA", int(i6e::api::KeyCode::KC_KANA)),
				value("KC_ABNT_C1", int(i6e::api::KeyCode::KC_ABNT_C1)),
				value("KC_CONVERT", int(i6e::api::KeyCode::KC_CONVERT)),
				value("KC_NOCONVERT", int(i6e::api::KeyCode::KC_NOCONVERT)),
				value("KC_YEN", int(i6e::api::KeyCode::KC_YEN)),
				value("KC_ABNT_C2", int(i6e::api::KeyCode::KC_ABNT_C2)),
				value("KC_NUMPADEQUALS", int(i6e::api::KeyCode::KC_NUMPADEQUALS)),
				value("KC_PREVTRACK", int(i6e::api::KeyCode::KC_PREVTRACK)),
				value("KC_AT", int(i6e::api::KeyCode::KC_AT)),
				value("KC_COLON", int(i6e::api::KeyCode::KC_COLON)),
				value("KC_UNDERLINE", int(i6e::api::KeyCode::KC_UNDERLINE)),
				value("KC_KANJI", int(i6e::api::KeyCode::KC_KANJI)),
				value("KC_STOP", int(i6e::api::KeyCode::KC_STOP)),
				value("KC_AX", int(i6e::api::KeyCode::KC_AX)),
				value("KC_UNLABELED", int(i6e::api::KeyCode::KC_UNLABELED)),
				value("KC_NEXTTRACK", int(i6e::api::KeyCode::KC_NEXTTRACK)),
				value("KC_NUMPADENTER", int(i6e::api::KeyCode::KC_NUMPADENTER)),
				value("KC_RCONTROL", int(i6e::api::KeyCode::KC_RCONTROL)),
				value("KC_MUTE", int(i6e::api::KeyCode::KC_MUTE)),
				value("KC_CALCULATOR", int(i6e::api::KeyCode::KC_CALCULATOR)),
				value("KC_PLAYPAUSE", int(i6e::api::KeyCode::KC_PLAYPAUSE)),
				value("KC_MEDIASTOP", int(i6e::api::KeyCode::KC_MEDIASTOP)),
				value("KC_VOLUMEDOWN", int(i6e::api::KeyCode::KC_VOLUMEDOWN)),
				value("KC_VOLUMEUP", int(i6e::api::KeyCode::KC_VOLUMEUP)),
				value("KC_WEBHOME", int(i6e::api::KeyCode::KC_WEBHOME)),
				value("KC_NUMPADCOMMA", int(i6e::api::KeyCode::KC_NUMPADCOMMA)),
				value("KC_DIVIDE", int(i6e::api::KeyCode::KC_DIVIDE)),
				value("KC_SYSRQ", int(i6e::api::KeyCode::KC_SYSRQ)),
				value("KC_RMENU", int(i6e::api::KeyCode::KC_RMENU)),
				value("KC_PAUSE", int(i6e::api::KeyCode::KC_PAUSE)),
				value("KC_HOME", int(i6e::api::KeyCode::KC_HOME)),
				value("KC_UP", int(i6e::api::KeyCode::KC_UP)),
				value("KC_PGUP", int(i6e::api::KeyCode::KC_PGUP)),
				value("KC_LEFT", int(i6e::api::KeyCode::KC_LEFT)),
				value("KC_RIGHT", int(i6e::api::KeyCode::KC_RIGHT)),
				value("KC_END", int(i6e::api::KeyCode::KC_END)),
				value("KC_DOWN", int(i6e::api::KeyCode::KC_DOWN)),
				value("KC_PGDOWN", int(i6e::api::KeyCode::KC_PGDOWN)),
				value("KC_INSERT", int(i6e::api::KeyCode::KC_INSERT)),
				value("KC_DELETE", int(i6e::api::KeyCode::KC_DELETE)),
				value("KC_LWIN", int(i6e::api::KeyCode::KC_LWIN)),
				value("KC_RWIN", int(i6e::api::KeyCode::KC_RWIN)),
				value("KC_APPS", int(i6e::api::KeyCode::KC_APPS)),
				value("KC_POWER", int(i6e::api::KeyCode::KC_POWER)),
				value("KC_SLEEP", int(i6e::api::KeyCode::KC_SLEEP)),
				value("KC_WAKE", int(i6e::api::KeyCode::KC_WAKE)),
				value("KC_WEBSEARCH", int(i6e::api::KeyCode::KC_WEBSEARCH)),
				value("KC_WEBFAVORITES", int(i6e::api::KeyCode::KC_WEBFAVORITES)),
				value("KC_WEBREFRESH", int(i6e::api::KeyCode::KC_WEBREFRESH)),
				value("KC_WEBSTOP", int(i6e::api::KeyCode::KC_WEBSTOP)),
				value("KC_WEBFORWARD", int(i6e::api::KeyCode::KC_WEBFORWARD)),
				value("KC_WEBBACK", int(i6e::api::KeyCode::KC_WEBBACK)),
				value("KC_MYCOMPUTER", int(i6e::api::KeyCode::KC_MYCOMPUTER)),
				value("KC_MAIL", int(i6e::api::KeyCode::KC_MAIL)),
				value("KC_MEDIASELECT", int(i6e::api::KeyCode::KC_MEDIASELECT)),
				value("KC_MBLeft", int(i6e::api::KeyCode::KC_MBLeft)),
				value("KC_MBRight", int(i6e::api::KeyCode::KC_MBRight)),
				value("KC_MBMiddle", int(i6e::api::KeyCode::KC_MBMiddle)),
				value("KC_MBButton3", int(i6e::api::KeyCode::KC_MBButton3)),
				value("KC_MBButton4", int(i6e::api::KeyCode::KC_MBButton4)),
				value("KC_MBButton5", int(i6e::api::KeyCode::KC_MBButton5)),
				value("KC_MBButton6", int(i6e::api::KeyCode::KC_MBButton6)),
				value("KC_MBButton7", int(i6e::api::KeyCode::KC_MBButton7))
			],

		def("setKeyMapping", &i6e::lua::input::setKeyMapping),
		def("removeKeyMapping", &i6e::lua::input::removeKeyMapping),
		def("getKeyMapping", &i6e::lua::input::getKeyMapping),
		def("subscribeKeyEvent", &i6e::lua::input::subscribeKeyEvent),
		def("unsubscribeKeyEvent", &i6e::lua::input::unsubscribeKeyEvent),
		def("resetInputSubSystem", &i6e::lua::input::resetInputSubSystem)
		;
}
