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

#include "i6engine/api/components/SoundListenerComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/AudioConfig.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
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

	std::pair<AddStrategy, int64_t> SoundListenerComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::REPLACE_DIS, 0);
	}

} /* namespace api */
} /* namespace i6e */
