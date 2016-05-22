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

#include "i6engine/api/components/ToggleWaynetComponent.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/manager/WaynetManager.h"

namespace i6e {
namespace api {

	ToggleWaynetComponent::ToggleWaynetComponent(int64_t id, const attributeMap & params) : Component(id, params), MessageSubscriberFacade(), _active(false) {
		_objFamilyID = components::ComponentTypes::ToggleWaynetComponent;
		_objComponentID = components::ComponentTypes::ToggleWaynetComponent;
	}

	void ToggleWaynetComponent::Init() {
		ISIXE_REGISTERMESSAGETYPE(messages::InputMessageType, ToggleWaynetComponent::News, this);
		addTicker();
	}

	void ToggleWaynetComponent::Tick() {
		processMessages();

		if (_active) {
			EngineController::GetSingleton().getWaynetManager()->showWaynet();
		}
	}

	void ToggleWaynetComponent::Finalize() {
		ISIXE_UNREGISTERMESSAGETYPE(messages::InputMessageType);
		removeTicker();
	}

	void ToggleWaynetComponent::News(const GameMessage::Ptr & msg) {
		if (msg->getMessageType() == messages::InputMessageType) {
			if (msg->getSubtype() == keyboard::KeyboardMessageTypes::KeyKeyboard) {
				input::Input_Keyboard_Update * iku = dynamic_cast<input::Input_Keyboard_Update *>(msg->getContent());
				if (!EngineController::GetSingleton().getGUIFacade()->getInputCaptured()) {
					std::string key = EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(iku->code);

					if (key == "toggleWaynet") {
						if (iku->pressed == api::KeyState::KEY_PRESSED) {
							_active = !_active;
						}
					}
				}
			}
		}
	}

} /* namespace api */
} /* namespace i6e */
