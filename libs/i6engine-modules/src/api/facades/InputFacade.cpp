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

#include "i6engine/api/facades/InputFacade.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	InputFacade::InputFacade() : _keymap(), _mbmap(), _lock() {}

	InputFacade::~InputFacade() {}

	// mouse methods
	void InputFacade::setMouseButtonMapping(const MouseButtonID id, const std::string & strAction) {
		boost::mutex::scoped_lock sl(_lock);
		_mbmap[id] = strAction;
	}

	std::string InputFacade::getMouseButtonMapping(const MouseButtonID id) const {
		boost::mutex::scoped_lock sl(_lock);
		mapMouseButtonMap::const_iterator iter = _mbmap.find(id);

		if (iter == _mbmap.end()) {
			return "NULL";
		}

		return iter->second;
	}

	// keyboard methods
	void InputFacade::setKeyMapping(const KeyCode id, const std::string & strAction) {
		boost::mutex::scoped_lock sl(_lock);
		_keymap[id] = strAction;
	}

	void InputFacade::removeKeyMapping(const KeyCode id) {
		boost::mutex::scoped_lock sl(_lock);
		_keymap.erase(id);
	}

	std::string InputFacade::getKeyMapping(const KeyCode id) const {
		boost::mutex::scoped_lock sl(_lock);
		mapKeymap::const_iterator iter = _keymap.find(id);

		if (iter == _keymap.end()) {
			return "NULL";
		}
		return iter->second;
	}

	void InputFacade::subscribeKeyEvent(const KeyCode name, const std::string & type, const boost::function<void(void)> & f) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::InputMessageType, input::InputSubscribeKeyEvent, core::Method::Create, new input::Input_SubscribeKeyEvent_Create(name, type, f), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void InputFacade::unsubscribeKeyEvent(const KeyCode name, const std::string & type) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::InputMessageType, input::InputSubscribeKeyEvent, core::Method::Delete, new input::Input_SubscribeKeyEvent_Delete(name, type), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void InputFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::InputMessageType, input::InputReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace api */
} /* namespace i6engine */
