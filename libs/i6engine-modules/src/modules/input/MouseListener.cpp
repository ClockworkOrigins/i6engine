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
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace modules {

	MouseListener::MouseListener() : _lastWheelPos() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	bool MouseListener::mouseMoved(const OIS::MouseEvent & objMouseEvent) {
		ASSERT_THREAD_SAFETY_FUNCTION

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouMouse, core::Method::Update, new api::input::Input_Mouse_Update(objMouseEvent.state.X.abs, objMouseEvent.state.Y.abs), i6engine::core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		if (objMouseEvent.state.Z.abs != _lastWheelPos) {
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouWheel, core::Method::Update, new api::input::Input_MouseWheel_Update((objMouseEvent.state.Z.abs - _lastWheelPos) / 120), i6engine::core::Subsystem::Input));
			_lastWheelPos = objMouseEvent.state.Z.abs;
		}

		return true;
	}

	bool MouseListener::mousePressed(const OIS::MouseEvent & objMouseEvent, OIS::MouseButtonID objButtonID) {
		ASSERT_THREAD_SAFETY_FUNCTION

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouButton, core::Method::Update, new api::input::Input_Button_Update(true, objButtonID), i6engine::core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		return true;
	}

	bool MouseListener::mouseReleased(const OIS::MouseEvent & objMouseEvent, OIS::MouseButtonID objButtonID) {
		ASSERT_THREAD_SAFETY_FUNCTION

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouButton, core::Method::Update, new api::input::Input_Button_Update(false, objButtonID), i6engine::core::Subsystem::Input);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		return true;
	}

} /* namespace modules */
} /* namespace i6engine */
