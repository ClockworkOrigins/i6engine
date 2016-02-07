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

#include "i6engine/api/components/AnimatedLuminousAppearanceComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
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
			params.insert(std::make_pair("diffuseDuration", std::to_string(_diffuseDuration)));
			for (auto it = _diffuseColours.begin(); it != _diffuseColours.end(); it++) {
				it->second.insertInMap("diffuseKey_" + std::to_string(it->first), params);
			}
		}
		if (_specularDuration > 0) {
			params.insert(std::make_pair("specularDuration", std::to_string(_specularDuration)));
			for (auto it = _specularColours.begin(); it != _specularColours.end(); it++) {
				it->second.insertInMap("specularKey_" + std::to_string(it->first), params);
			}
		}
		if (_attenuationDuration > 0) {
			params.insert(std::make_pair("attenuationDuration", std::to_string(_attenuationDuration)));
			for (auto it = _attenuations.begin(); it != _attenuations.end(); it++) {
				it->second.insertInMap("attenuationKey_" + std::to_string(it->first), params);
			}
		}
		return params;
	}

	std::vector<componentOptions> AnimatedLuminousAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result = LuminousAppearanceComponent::getComponentOptions();
		return result;
	}

} /* namespace api */
} /* namespace i6engine */
