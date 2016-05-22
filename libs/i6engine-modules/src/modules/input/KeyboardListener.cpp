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

#include "i6engine/modules/input/KeyboardListener.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/thread.hpp"

namespace i6e {
namespace modules {

	KeyboardListener::KeyboardListener() : _keyStates(), _keyTexts(), _objInputKeyFunctions() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	bool KeyboardListener::keyPressed(const OIS::KeyEvent & objKeyEvent) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_keyStates[objKeyEvent.key] == api::KeyState::KEY_RELEASED) {
			_keyStates[objKeyEvent.key] = api::KeyState::KEY_PRESSED;
			_keyTexts[objKeyEvent.key] = objKeyEvent.text;

			triggerKeyFunction(api::KeyCode(objKeyEvent.key), api::KeyState::KEY_PRESSED);
		} else if (_keyStates[objKeyEvent.key] == api::KeyState::KEY_PRESSED) {
			_keyStates[objKeyEvent.key] = api::KeyState::KEY_HOLD;

			triggerKeyFunction(api::KeyCode(objKeyEvent.key), api::KeyState::KEY_HOLD);
		}

		uint32_t text = objKeyEvent.text;

		if (_keyStates[OIS::KeyCode(api::KeyCode::KC_LSHIFT)] == api::KeyState::KEY_PRESSED || _keyStates[OIS::KeyCode(api::KeyCode::KC_LSHIFT)] == api::KeyState::KEY_HOLD || _keyStates[OIS::KeyCode(api::KeyCode::KC_RSHIFT)] == api::KeyState::KEY_PRESSED || _keyStates[OIS::KeyCode(api::KeyCode::KC_RSHIFT)] == api::KeyState::KEY_HOLD) {
			if (text >= 'a' && text <= 'z') {
				text -= 'a' - 'A';
			}
		}

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(_keyStates[objKeyEvent.key], api::KeyCode(objKeyEvent.key), text), core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		return true;
	}

	bool KeyboardListener::keyReleased(const OIS::KeyEvent & objKeyEvent) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_keyStates[objKeyEvent.key] == api::KeyState::KEY_HOLD || _keyStates[objKeyEvent.key] == api::KeyState::KEY_PRESSED) {
			_keyStates[objKeyEvent.key] = api::KeyState::KEY_RELEASED;

			triggerKeyFunction(api::KeyCode(objKeyEvent.key), api::KeyState::KEY_RELEASED);
		}

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(_keyStates[objKeyEvent.key], api::KeyCode(objKeyEvent.key), objKeyEvent.text), core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		return true;
	}

	void KeyboardListener::setKeyFunction(const api::KeyCode name, const api::KeyState type, const boost::function<void(void)> & f) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objInputKeyFunctions[std::make_pair(name, type)] = f;
	}

	void KeyboardListener::removeKeyFunction(const api::KeyCode name, const api::KeyState type) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objInputKeyFunctions.erase(std::make_pair(name, type));
	}

	void KeyboardListener::triggerKeyFunction(const api::KeyCode keyCode, const api::KeyState type) {
		ASSERT_THREAD_SAFETY_FUNCTION
		InputKeyFunctions::const_iterator iter = _objInputKeyFunctions.find(std::make_pair(keyCode, type));
		if (iter == _objInputKeyFunctions.end()) {
			// keyCode is register for another event
			return;
		}

		std::thread(iter->second).detach();
	}

	void KeyboardListener::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (size_t i = 0; i < _keyStates.size(); i++) {
			if (_keyStates[i] == api::KeyState::KEY_PRESSED) {
				_keyStates[i] = api::KeyState::KEY_HOLD;
			} else if (_keyStates[i] == api::KeyState::KEY_HOLD) {
				triggerKeyFunction(api::KeyCode(i), api::KeyState::KEY_HOLD);

				uint32_t text = uint32_t(_keyTexts[i]);

				if (_keyStates[OIS::KeyCode(api::KeyCode::KC_LSHIFT)] == api::KeyState::KEY_PRESSED || _keyStates[OIS::KeyCode(api::KeyCode::KC_LSHIFT)] == api::KeyState::KEY_HOLD || _keyStates[OIS::KeyCode(api::KeyCode::KC_RSHIFT)] == api::KeyState::KEY_PRESSED || _keyStates[OIS::KeyCode(api::KeyCode::KC_RSHIFT)] == api::KeyState::KEY_HOLD) {
					if (text >= 'a' && text <= 'z') {
						text -= 'a' - 'A';
					}
				}

				api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(_keyStates[i], api::KeyCode(i), text), core::Subsystem::Input);

				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
