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

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	SoundComponent::SoundComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _offset(Vec3(params, "offset")), _direction(Vec3(params, "direction")), _file(params.find("file")->second), _looping(boost::lexical_cast<bool>(params.find("looping")->second)), _maxDist(boost::lexical_cast<double>(params.find("maxDist")->second)), _psc(), _cacheable(boost::lexical_cast<bool>(params.find("cache")->second)) {
		Component::_objFamilyID = components::SoundComponent;
		Component::_objComponentID = components::SoundComponent;
	}

	SoundComponent::~SoundComponent() {
	}

	ComPtr SoundComponent::createC(int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("SoundComponent", "offset not set!", params.find("offset") != params.end());
		ISIXE_THROW_API_COND("SoundComponent", "direction not set!", params.find("direction") != params.end());
		ISIXE_THROW_API_COND("SoundComponent", "file not set!", params.find("file") != params.end());
		ISIXE_THROW_API_COND("SoundComponent", "looping not set!", params.find("looping") != params.end());
		ISIXE_THROW_API_COND("SoundComponent", "maxDist not set!", params.find("maxDist") != params.end());
		ISIXE_THROW_API_COND("SoundComponent", "cache not set!", params.find("cache") != params.end());
		return utils::make_shared<SoundComponent, Component>(id, params);
	}

	void SoundComponent::Init() {
		addTicker();

		auto psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);
		_position = psc->getPosition();
		Vec3 position = psc->getPosition() + math::rotateVector(_offset, psc->getRotation());
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioNode, core::Method::Create, new audio::Audio_Node_Create(getID(), _file, _looping, _maxDist, position, _direction, _cacheable), i6engine::core::Subsystem::Object));
		_psc = psc;
	}

	void SoundComponent::Finalize() {
		removeTicker();

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioNode, core::Method::Delete, new audio::Audio_Node_Delete(getID()), i6engine::core::Subsystem::Object));
	}

	void SoundComponent::Tick() {
		auto psc = _psc.get();
		if (psc->getPosition() != _position) {
			_position = psc->getPosition();
			Vec3 position = _position + math::rotateVector(_offset, psc->getRotation());
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioPosition, core::Method::Update, new audio::Audio_Position_Update(getID(), position), i6engine::core::Subsystem::Object));
		}
	}

	attributeMap SoundComponent::synchronize() const {
		attributeMap params;
		_offset.insertInMap("offset", params);
		_direction.insertInMap("direction", params);
		params["file"] = _file;
		params["looping"] = boost::lexical_cast<std::string>(_looping);
		params["maxDist"] = boost::lexical_cast<std::string>(_maxDist);
		params["cache"] = boost::lexical_cast<std::string>(_cacheable);
		return params;
	}

	std::pair<AddStrategy, int64_t> SoundComponent::howToAdd(const ComPtr & comp) const {
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
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Looping", [this]() {
			return boost::lexical_cast<std::string>(_looping);
		}, [this](std::string s) {
			try {
				_looping = boost::lexical_cast<bool>(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) update change
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Max. Dist", [this]() {
			return boost::lexical_cast<std::string>(_maxDist);
		}, [this](std::string s) {
			try {
				_maxDist = boost::lexical_cast<double>(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) update change
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Offset", [this]() {
			return _position.toString();
		}, [this](std::string s) {
			try {
				_position = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) update change
			return true;
		}));
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
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Cache", [this]() {
			return boost::lexical_cast<std::string>(_cacheable);
		}, [this](std::string s) {
			try {
				_cacheable = boost::lexical_cast<bool>(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) update change
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
