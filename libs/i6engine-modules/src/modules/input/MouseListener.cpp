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

#include "i6engine/modules/input/MouseListener.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace modules {

	MouseListener::MouseListener() : _lastWheelPos(), _keyStates(), _objInputKeyFunctions() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	bool MouseListener::mouseMoved(const OIS::MouseEvent & objMouseEvent) {
		ASSERT_THREAD_SAFETY_FUNCTION

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouMouse, core::Method::Update, new api::input::Input_Mouse_Update(objMouseEvent.state.X.abs, objMouseEvent.state.Y.abs), core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		if (objMouseEvent.state.Z.abs != _lastWheelPos) {
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouWheel, core::Method::Update, new api::input::Input_MouseWheel_Update((objMouseEvent.state.Z.abs - _lastWheelPos) / 120), core::Subsystem::Input));
			_lastWheelPos = objMouseEvent.state.Z.abs;
		}

		return true;
	}

	bool MouseListener::mousePressed(const OIS::MouseEvent &, OIS::MouseButtonID objButtonID) {
		ASSERT_THREAD_SAFETY_FUNCTION
		api::KeyCode kc = api::KeyCode(size_t(api::KeyCode::MOUSEBUTTONS) + objButtonID - OIS::MB_Left + 1);
		if (_keyStates[size_t(kc)] == api::KeyState::KEY_RELEASED) {
			_keyStates[size_t(kc)] = api::KeyState::KEY_PRESSED;
			triggerKeyFunction(kc, api::KeyState::KEY_PRESSED);
		} else if (_keyStates[size_t(kc)] == api::KeyState::KEY_PRESSED) {
			_keyStates[size_t(kc)] = api::KeyState::KEY_HOLD;
			triggerKeyFunction(kc, api::KeyState::KEY_HOLD);
		}

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_PRESSED, kc, 0x0), core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		return true;
	}

	bool MouseListener::mouseReleased(const OIS::MouseEvent &, OIS::MouseButtonID objButtonID) {
		ASSERT_THREAD_SAFETY_FUNCTION
		api::KeyCode kc = api::KeyCode(size_t(api::KeyCode::MOUSEBUTTONS) + objButtonID - OIS::MB_Left + 1);
		if (_keyStates[size_t(kc)] == api::KeyState::KEY_HOLD || _keyStates[size_t(kc)] == api::KeyState::KEY_PRESSED) {
			_keyStates[size_t(kc)] = api::KeyState::KEY_RELEASED;
			triggerKeyFunction(kc, api::KeyState::KEY_RELEASED);
		}

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_RELEASED, kc, 0x0), core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		return true;
	}

	void MouseListener::setKeyFunction(const api::KeyCode name, const api::KeyState type, const boost::function<void(void)> & f) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objInputKeyFunctions[std::make_pair(name, type)] = f;
	}

	void MouseListener::removeKeyFunction(const api::KeyCode name, const api::KeyState type) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objInputKeyFunctions.erase(std::make_pair(name, type));
	}

	void MouseListener::triggerKeyFunction(const api::KeyCode keyCode, const api::KeyState type) {
		ASSERT_THREAD_SAFETY_FUNCTION
		InputKeyFunctions::const_iterator iter = _objInputKeyFunctions.find(std::make_pair(keyCode, type));
		if (iter == _objInputKeyFunctions.end()) {
			// keyCode is register for another event
			return;
		}

		std::thread(iter->second).detach();
	}

	void MouseListener::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (size_t i = 0; i < _keyStates.size(); i++) {
			if (_keyStates[i] == api::KeyState::KEY_PRESSED) {
				_keyStates[i] = api::KeyState::KEY_HOLD;
			} else if (_keyStates[i] == api::KeyState::KEY_HOLD) {
				triggerKeyFunction(api::KeyCode(i), api::KeyState::KEY_HOLD);

				api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(_keyStates[i], api::KeyCode(i), 0x0), core::Subsystem::Input);

				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
