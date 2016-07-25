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

#include "i6engine/api/facades/InputFacade.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace api {

	InputFacade::InputFacade() : _keymap(), _lock() {}

	InputFacade::~InputFacade() {}

	// keyboard methods
	void InputFacade::setKeyMapping(const KeyCode id, const std::string & strAction) {
		std::lock_guard<std::mutex> sl(_lock);
		_keymap[id] = strAction;
	}

	void InputFacade::removeKeyMapping(const KeyCode id) {
		std::lock_guard<std::mutex> sl(_lock);
		_keymap.erase(id);
	}

	std::string InputFacade::getKeyMapping(const KeyCode id) const {
		std::lock_guard<std::mutex> sl(_lock);
		mapKeymap::const_iterator iter = _keymap.find(id);

		if (iter == _keymap.end()) {
			return "NULL";
		}
		return iter->second;
	}

	void InputFacade::subscribeKeyEvent(const KeyCode name, const KeyState type, const std::function<void(void)> & f) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::InputMessageType, input::InputSubscribeKeyEvent, core::Method::Create, new input::Input_SubscribeKeyEvent_Create(name, type, f), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void InputFacade::unsubscribeKeyEvent(const KeyCode name, const KeyState type) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::InputMessageType, input::InputSubscribeKeyEvent, core::Method::Delete, new input::Input_SubscribeKeyEvent_Delete(name, type), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void InputFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::InputMessageType, input::InputReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace api */
} /* namespace i6e */
