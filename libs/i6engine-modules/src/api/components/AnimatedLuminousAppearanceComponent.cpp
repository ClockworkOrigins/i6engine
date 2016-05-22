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

#include "i6engine/api/components/AnimatedLuminousAppearanceComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6e {
namespace api {

	AnimatedLuminousAppearanceComponent::AnimatedLuminousAppearanceComponent(const int64_t id, const attributeMap & params) : LuminousAppearanceComponent(id, params), _startTime(), _diffuseDuration(0), _diffuseColours(), _specularDuration(0), _specularColours(), _attenuationDuration(0), _attenuations() {
		Component::_objComponentID = components::AnimatedLuminousAppearanceComponent;

		parseAttribute<false>(params, "diffuseDuration", _diffuseDuration);
		parseAttribute<false>(params, "specularDuration", _specularDuration);
		parseAttribute<false>(params, "attenuationDuration", _attenuationDuration);

		if (_diffuseDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("diffuseKey_") != std::string::npos) {
					Vec3 diffuseColour;
					parseAttribute<true>(params, it->first, diffuseColour);
					_diffuseColours.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), diffuseColour));
				}
			}
		}
		if (_specularDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("specularKey_") != std::string::npos) {
					Vec3 specularColour;
					parseAttribute<true>(params, it->first, specularColour);
					_specularColours.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), specularColour));
				}
			}
		}
		if (_attenuationDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("attenuationKey_") != std::string::npos) {
					Vec4 attenuation;
					parseAttribute<true>(params, it->first, attenuation);
					_attenuations.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), attenuation));
				}
			}
		}
	}

	AnimatedLuminousAppearanceComponent::~AnimatedLuminousAppearanceComponent() {
	}

	void AnimatedLuminousAppearanceComponent::Tick() {
		if (_diffuseDuration > 0 && _diffuseColours.size() > 1) {
			setDiffuseColor(interpolate(_diffuseDuration, _diffuseColours));
		}
		if (_specularDuration > 0 && _specularColours.size() > 1) {
			setSpecularColor(interpolate(_specularDuration, _specularColours));
		}
		if (_attenuationDuration > 0 && _attenuations.size() > 1) {
			setAttenuation(interpolate(_attenuationDuration, _attenuations));
		}
	}

	void AnimatedLuminousAppearanceComponent::Init() {
		LuminousAppearanceComponent::Init();
		addTicker();
		_startTime = api::EngineController::GetSingleton().getCurrentTime();
	}

	void AnimatedLuminousAppearanceComponent::Finalize() {
		LuminousAppearanceComponent::Finalize();
		removeTicker();
	}

	attributeMap AnimatedLuminousAppearanceComponent::synchronize() const {
		attributeMap params = LuminousAppearanceComponent::synchronize();
		if (_diffuseDuration > 0) {
			writeAttribute(params, "diffuseDuration", _diffuseDuration);
			for (auto it = _diffuseColours.begin(); it != _diffuseColours.end(); it++) {
				writeAttribute(params, "diffuseKey_" + std::to_string(it->first), it->second);
			}
		}
		if (_specularDuration > 0) {
			writeAttribute(params, "specularDuration", _specularDuration);
			for (auto it = _specularColours.begin(); it != _specularColours.end(); it++) {
				writeAttribute(params, "specularKey_" + std::to_string(it->first), it->second);
			}
		}
		if (_attenuationDuration > 0) {
			writeAttribute(params, "attenuationDuration", _attenuationDuration);
			for (auto it = _attenuations.begin(); it != _attenuations.end(); it++) {
				writeAttribute(params, "attenuationKey_" + std::to_string(it->first), it->second);
			}
		}
		return params;
	}

	std::vector<componentOptions> AnimatedLuminousAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result = LuminousAppearanceComponent::getComponentOptions();
		return result;
	}

} /* namespace api */
} /* namespace i6e */
