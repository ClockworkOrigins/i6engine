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

#include "i6engine/api/components/ToggleWaynetComponent.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/manager/WaynetManager.h"

namespace i6engine {
namespace api {

	ToggleWaynetComponent::ToggleWaynetComponent(int64_t id, const attributeMap & params) : Component(id, params), MessageSubscriberFacade(), _active(false) {
		_objFamilyID = components::ComponentTypes::ToggleWaynetComponent;
		_objComponentID = components::ComponentTypes::ToggleWaynetComponent;
	}

	ComPtr ToggleWaynetComponent::createC(int64_t id, const attributeMap & params) {
		return utils::make_shared<ToggleWaynetComponent, Component>(id, params);
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
} /* namespace i6engine */