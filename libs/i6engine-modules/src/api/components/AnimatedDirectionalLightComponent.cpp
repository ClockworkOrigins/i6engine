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

#include "i6engine/api/components/AnimatedDirectionalLightComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6e {
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
			writeAttribute(params, "directionDuration", _directionDuration);
			for (auto it = _directions.begin(); it != _directions.end(); it++) {
				writeAttribute(params, "directionKey_" + std::to_string(it->first), it->second);
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
} /* namespace i6e */
