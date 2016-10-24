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

#include "i6engine/api/components/SoundComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/AudioConfig.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	SoundComponent::SoundComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _offset(), _direction(), _file(), _looping(), _maxDist(), _psc(), _cacheable(), _category("") {
		Component::_objFamilyID = components::SoundComponent;
		Component::_objComponentID = components::SoundComponent;

		parseAttribute<true>(params, "offset", _offset);
		parseAttribute<true>(params, "direction", _direction);
		parseAttribute<true>(params, "file", _file);
		parseAttribute<true>(params, "looping", _looping);
		parseAttribute<true>(params, "maxDist", _maxDist);
		parseAttribute<true>(params, "cache", _cacheable);
		parseAttribute<false>(params, "category", _category);
	}

	SoundComponent::~SoundComponent() {
	}

	void SoundComponent::Init() {
		addTicker();

		auto psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);
		_position = psc->getPosition();
		Vec3 position = psc->getPosition() + math::rotateVector(_offset, psc->getRotation());
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioNode, core::Method::Create, new audio::Audio_Node_Create(getID(), _file, _looping, _maxDist, position, _direction, _cacheable, _category), core::Subsystem::Object));
		_psc = psc;
	}

	void SoundComponent::Finalize() {
		removeTicker();

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioNode, core::Method::Delete, new audio::Audio_Node_Delete(getID()), core::Subsystem::Object));
	}

	void SoundComponent::Tick() {
		auto psc = _psc.get();
		if (psc->getPosition() != _position) {
			_position = psc->getPosition();
			Vec3 position = _position + math::rotateVector(_offset, psc->getRotation());
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioPosition, core::Method::Update, new audio::Audio_Position_Update(getID(), position), core::Subsystem::Object));
		}
	}

	attributeMap SoundComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "offset", _offset);
		writeAttribute(params, "direction", _direction);
		writeAttribute(params, "file", _file);
		writeAttribute(params, "looping", _looping);
		writeAttribute(params, "maxDist", _maxDist);
		writeAttribute(params, "cache", _cacheable);

		if (!_category.empty()) {
			writeAttribute(params, "category", _category);
		}
		return params;
	}

	std::pair<AddStrategy, int64_t> SoundComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> SoundComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "File", [this]() {
			return _file;
		}, [this](std::string s) {
			_file = s;
			// TODO: (Daniel) update change
			return true;
		}, "String"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Looping", [this]() {
			return std::to_string(_looping);
		}, [this](std::string s) {
			_looping = std::stoi(s) == 1;
			// TODO: (Daniel) update change
			return true;
		}, "Bool"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Max. Dist", [this]() {
			return std::to_string(_maxDist);
		}, [this](std::string s) {
			_maxDist = std::stod(s);
			// TODO: (Daniel) update change
			return true;
		}, "Double"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Offset", [this]() {
			return _position.toString();
		}, [this](std::string s) {
			_position = Vec3(s);
			// TODO: (Daniel) update change
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Direction", [this]() {
			return _direction.toString();
		}, [this](std::string s) {
			try {
				_direction = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) update change
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Cache", [this]() {
			return std::to_string(_cacheable);
		}, [this](std::string s) {
			_cacheable = std::stoi(s) == 1;
			// TODO: (Daniel) update change
			return true;
		}, "Bool"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
