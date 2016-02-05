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

#include "i6engine/api/components/ParticleEmitterComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	ParticleEmitterComponent::ParticleEmitterComponent(const int64_t id, const attributeMap & params) : Component(id, params), _emitterName(), _pos(), _fadeOut(false), _fadeOutCooldown(0) {
		Component::_objFamilyID = components::ParticleEmitterComponent;
		Component::_objComponentID = components::ParticleEmitterComponent;

		parseAttribute<true>(params, "particleEmitter", _emitterName);
		parseAttribute<false>(params, "pos", _pos);
		parseAttribute<false>(params, "fadeOut", _fadeOut);

		if (params.find("pos") != params.end()) {
			_pos = Vec3(params, "pos");
		}
		if (_fadeOut) {
			parseAttribute<true>(params, "fadeOutCooldown", _fadeOutCooldown);
		}
	}

	ParticleEmitterComponent::~ParticleEmitterComponent() {
	}

	void ParticleEmitterComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticle, core::Method::Create, new graphics::Graphics_Particle_Create(_objOwnerID, getID(), _emitterName, _pos), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ParticleEmitterComponent::Finalize() {
		if (_fadeOut) {
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticleFadeOut, core::Method::Update, new graphics::Graphics_ParticleFadeOut_Update(_objOwnerID, getID()), core::Subsystem::Object));
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticle, core::Method::Delete, new graphics::Graphics_Particle_Delete(_objOwnerID, getID()), core::Subsystem::Object);
			EngineController::GetSingleton().registerTimer(_fadeOutCooldown, [msg]() {
				EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				return false;
			}, false, core::JobPriorities::Prio_Medium);
		} else {
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticle, core::Method::Delete, new graphics::Graphics_Particle_Delete(_objOwnerID, getID()), core::Subsystem::Object);
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		}
	}

	attributeMap ParticleEmitterComponent::synchronize() const {
		attributeMap params;
		params["particleEmitter"] = _emitterName;
		params["fadeOut"] = std::to_string(_fadeOut);
		params["fadeOutCooldown"] = std::to_string(_fadeOutCooldown);

		if (_pos != Vec3::ZERO) {
			_pos.insertInMap("pos", params);
		}

		return params;
	}

	std::pair<AddStrategy, int64_t> ParticleEmitterComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> ParticleEmitterComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Emitter", [this]() {
			return _emitterName;
		}, [this](std::string s) {
			_emitterName = s;
			// TODO: (Daniel) send Update
			return true;
		}, "String"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Position", [this]() {
			return _pos.toString();
		}, [this](std::string s) {
			_pos = Vec3(s);
			// TODO: (Daniel) send Update
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "FadeOut", [this]() {
			return std::to_string(_fadeOut);
		}, [this](std::string s) {
			_fadeOut = s == "1";
			return true;
		}, "Bool"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "FadeOut Cooldown", [this]() {
			return std::to_string(_fadeOutCooldown);
		}, [this](std::string s) {
			_fadeOutCooldown = std::stoul(s);
			return true;
		}, "Integer"));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
