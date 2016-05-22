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

#include "i6engine/api/components/AnimatedSpotLightComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6e {
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
			writeAttribute(params, "directionDuration", _directionDuration);
			for (auto it = _directions.begin(); it != _directions.end(); it++) {
				writeAttribute(params, "directionKey_" + std::to_string(it->first), it->second);
			}
		}
		if (_innerRangeDuration > 0) {
			writeAttribute(params, "innerRangeDuration", _innerRangeDuration);
			for (auto it = _innerRanges.begin(); it != _innerRanges.end(); it++) {
				writeAttribute(params, "innerRangeKey_" + std::to_string(it->first), it->second);
			}
		}
		if (_outerRangeDuration > 0) {
			writeAttribute(params, "innerRangeDuration", _outerRangeDuration);
			for (auto it = _outerRanges.begin(); it != _outerRanges.end(); it++) {
				writeAttribute(params, "outerRangeKey_" + std::to_string(it->first), it->second);
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
} /* namespace i6e */
