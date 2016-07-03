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

#include "i6engine/api/components/ParticleEmitterComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace api {

	ParticleEmitterComponent::ParticleEmitterComponent(const int64_t id, const attributeMap & params) : Component(id, params), _emitterName(), _pos(), _scale(1.0, 1.0, 1.0), _fadeOut(false), _fadeOutCooldown(0) {
		Component::_objFamilyID = components::ParticleEmitterComponent;
		Component::_objComponentID = components::ParticleEmitterComponent;

		parseAttribute<true>(params, "particleEmitter", _emitterName);
		parseAttribute<false>(params, "pos", _pos);
		parseAttribute<false>(params, "scale", _scale);
		parseAttribute<false>(params, "fadeOut", _fadeOut);

		if (_fadeOut) {
			parseAttribute<true>(params, "fadeOutCooldown", _fadeOutCooldown);
		}
	}

	ParticleEmitterComponent::~ParticleEmitterComponent() {
	}

	void ParticleEmitterComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticle, core::Method::Create, new graphics::Graphics_Particle_Create(_objOwnerID, getID(), _emitterName, _pos, _scale), core::Subsystem::Object);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void ParticleEmitterComponent::Finalize() {
		if (_fadeOut) {
			i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticleFadeOut, core::Method::Update, new graphics::Graphics_ParticleFadeOut_Update(_objOwnerID, getID()), core::Subsystem::Object));
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticle, core::Method::Delete, new graphics::Graphics_Particle_Delete(_objOwnerID, getID()), core::Subsystem::Object);
			i6eEngineController->registerTimer(_fadeOutCooldown, [msg]() {
				i6eMessagingFacade->deliverMessage(msg);
				return false;
			}, false, core::JobPriorities::Prio_Medium);
		} else {
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticle, core::Method::Delete, new graphics::Graphics_Particle_Delete(_objOwnerID, getID()), core::Subsystem::Object);
			i6eMessagingFacade->deliverMessage(msg);
		}
	}

	void ParticleEmitterComponent::setScale(const Vec3 & scale) {
		_scale = scale;
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraParticleScale, core::Method::Update, new graphics::Graphics_ParticleScale_Update(_objOwnerID, getID(), scale), core::Subsystem::Object));
	}

	attributeMap ParticleEmitterComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "particleEmitter", _emitterName);
		if (_fadeOut) {
			writeAttribute(params, "fadeOut", _fadeOut);
			writeAttribute(params, "fadeOutCooldown", _fadeOutCooldown);
		}
		if (_pos.isValid()) {
			writeAttribute(params, "pos", _pos);
		}
		if (_scale.isValid()) {
			writeAttribute(params, "scale", _scale);
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
		result.push_back(std::make_tuple(AccessState::READWRITE, "Scale", [this]() {
			return _scale.toString();
		}, [this](std::string s) {
			_scale = Vec3(s);
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
} /* namespace i6e */
