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

#include "i6engine/api/components/SoundListenerComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/AudioConfig.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	SoundListenerComponent::SoundListenerComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _rotation(), _psc() {
		Component::_objFamilyID = components::SoundListenerComponent;
		Component::_objComponentID = components::SoundListenerComponent;
	}

	SoundListenerComponent::~SoundListenerComponent() {
	}

	void SoundListenerComponent::Init() {
		addTicker();

		_psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);
	}

	void SoundListenerComponent::Finalize() {
		removeTicker();
	}

	void SoundListenerComponent::Tick() {
		auto psc = _psc.get();
		if (psc->getPosition() != _position) {
			_position = psc->getPosition();
			_rotation = psc->getRotation();
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioListener, core::Method::Update, new audio::Audio_Listener_Update(_position, _rotation, psc->getLinearVelocity()), core::Subsystem::Object));
		} else if (psc->getRotation() != _rotation) {
			_position = psc->getPosition();
			_rotation = psc->getRotation();
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioListener, core::Method::Update, new audio::Audio_Listener_Update(_position, _rotation, psc->getLinearVelocity()), core::Subsystem::Object));
		}
	}

	attributeMap SoundListenerComponent::synchronize() const {
		return attributeMap();
	}

	std::pair<AddStrategy, int64_t> SoundListenerComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::REPLACE_DIS, 0);
	}

} /* namespace api */
} /* namespace i6engine */
