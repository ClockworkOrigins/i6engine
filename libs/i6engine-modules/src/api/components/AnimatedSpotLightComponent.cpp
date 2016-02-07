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

#include "i6engine/api/components/AnimatedSpotLightComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
namespace api {

	AnimatedSpotLightComponent::AnimatedSpotLightComponent(const int64_t id, const attributeMap & params) : AnimatedLuminousAppearanceComponent(id, params), _directionDuration(0), _directions(), _innerRangeDuration(0), _innerRanges(), _outerRangeDuration(0), _outerRanges() {
		Component::_objComponentID = components::AnimatedSpotLightComponent;

		parseAttribute<false>(params, "directionDuration", _directionDuration);
		parseAttribute<false>(params, "innerRangeDuration", _innerRangeDuration);
		parseAttribute<false>(params, "outerRangeDuration", _outerRangeDuration);

		if (_directionDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("directionKey_") != std::string::npos) {
					Vec3 direction;
					parseAttribute<true>(params, it->first, direction);
					_directions.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), direction));
				}
			}
		}
		if (_innerRangeDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("innerRangeKey_") != std::string::npos) {
					double innerRange;
					parseAttribute<true>(params, it->first, innerRange);
					_innerRanges.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), innerRange));
				}
			}
		}
		if (_outerRangeDuration > 0) {
			for (auto it = params.begin(); it != params.end(); it++) {
				if (it->first.find("outerRangeKey_") != std::string::npos) {
					double outerRange;
					parseAttribute<true>(params, it->first, outerRange);
					_outerRanges.insert(std::make_pair(std::stod(utils::split(it->first, "_").back()), outerRange));
				}
			}
		}
	}

	AnimatedSpotLightComponent::~AnimatedSpotLightComponent() {
	}

	attributeMap AnimatedSpotLightComponent::synchronize() const {
		attributeMap params = AnimatedLuminousAppearanceComponent::synchronize();

		if (_directionDuration > 0) {
			params.insert(std::make_pair("directionDuration", std::to_string(_directionDuration)));
			for (auto it = _directions.begin(); it != _directions.end(); it++) {
				it->second.insertInMap("directionKey_" + std::to_string(it->first), params);
			}
		}
		if (_innerRangeDuration > 0) {
			params.insert(std::make_pair("innerRangeDuration", std::to_string(_innerRangeDuration)));
			for (auto it = _innerRanges.begin(); it != _innerRanges.end(); it++) {
				params.insert(std::make_pair("innerRangeKey_" + std::to_string(it->first), std::to_string(it->second)));
			}
		}
		if (_outerRangeDuration > 0) {
			params.insert(std::make_pair("innerRangeDuration", std::to_string(_outerRangeDuration)));
			for (auto it = _outerRanges.begin(); it != _outerRanges.end(); it++) {
				params.insert(std::make_pair("outerRangeKey_" + std::to_string(it->first), std::to_string(it->second)));
			}
		}

		return params;
	}

	void AnimatedSpotLightComponent::Tick() {
		AnimatedLuminousAppearanceComponent::Tick();
		if (_directionDuration > 0 && _directions.size() > 1) {
			setDirection(interpolate(_directionDuration, _directions));
		}
		if (_innerRangeDuration > 0 && _innerRanges.size() > 1) {
			setSpotLightInnerRange(interpolate(_innerRangeDuration, _innerRanges));
		}
		if (_outerRangeDuration > 0 && _outerRanges.size() > 1) {
			setSpotLightOuterRange(interpolate(_outerRangeDuration, _outerRanges));
		}
	}

	std::vector<componentOptions> AnimatedSpotLightComponent::getComponentOptions() {
		std::vector<componentOptions> result = AnimatedLuminousAppearanceComponent::getComponentOptions();
		return result;
	}

} /* namespace api */
} /* namespace i6engine */
