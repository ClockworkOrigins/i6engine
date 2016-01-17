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

#include "i6engine/api/components/AnimatedDirectionalLightComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
namespace api {

	AnimatedDirectionalLightComponent::AnimatedDirectionalLightComponent(const int64_t id, const attributeMap & params) : AnimatedLuminousAppearanceComponent(id, params), _directionDuration(0), _directions() {
		Component::_objComponentID = components::AnimatedDirectionalLightComponent;

		parseAttribute<false>(params, "directionDuration", _directionDuration);

		if (_directionDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("directionKey_") != std::string::npos) {
					Vec3 direction;
					parseAttribute<true>(params, it->first, direction);
					_directions.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), direction));
				}
			}
		}
	}

	AnimatedDirectionalLightComponent::~AnimatedDirectionalLightComponent() {
	}

	attributeMap AnimatedDirectionalLightComponent::synchronize() const {
		attributeMap params = AnimatedLuminousAppearanceComponent::synchronize();
		if (_directionDuration > 0) {
			params.insert(std::make_pair("directionDuration", std::to_string(_directionDuration)));
			for (auto it = _directions.begin(); it != _directions.end(); it++) {
				it->second.insertInMap("directionKey_" + std::to_string(it->first), params);
			}
		}
		return params;
	}

	void AnimatedDirectionalLightComponent::Tick() {
		AnimatedLuminousAppearanceComponent::Tick();
		if (_directionDuration > 0 && _directions.size() > 1) {
			setDirection(interpolate(_directionDuration, _directions));
		}
	}

	std::vector<componentOptions> AnimatedDirectionalLightComponent::getComponentOptions() {
		std::vector<componentOptions> result = AnimatedLuminousAppearanceComponent::getComponentOptions();
		return result;
	}

} /* namespace api */
} /* namespace i6engine */
