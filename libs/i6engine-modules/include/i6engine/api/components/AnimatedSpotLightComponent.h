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

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_ANIMATEDSPOTLIGHTCOMPONENT_H_
#define __I6ENGINE_API_ANIMATEDSPOTLIGHTCOMPONENT_H_

#include "i6engine/api/components/AnimatedLuminousAppearanceComponent.h"

namespace i6e {
namespace api {

	/**
	 * \class AnimatedSpotLightComponent
	 * \brief Tells the engine that this GameObject is emitting light.
	 * For creating a AnimatedSpotLightComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | directionDuration | no | int | duration of one direction interpolation cycle | yes |
	 * | directionKey_<timePoint> | no | Vec3 | direction at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 * | innerRangeDuration | no | int | duration of one inner range interpolation cycle | yes |
	 * | innerRangeKey_<timePoint> | no | double | inner range at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 * | outerRangeDuration | no | int | duration of one outer range interpolation cycle | yes |
	 * | outerRangeKey_<timePoint> | no | double | outer range at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 */
	class ISIXE_MODULES_API AnimatedSpotLightComponent : public AnimatedLuminousAppearanceComponent {
	public:
		/**
		 * \brief Constructor sets ID and component
		 */
		AnimatedSpotLightComponent(const int64_t id, const attributeMap & params);

		~AnimatedSpotLightComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "AnimatedSpotLight";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		uint64_t _directionDuration;
		std::map<double, Vec3> _directions;
		uint64_t _innerRangeDuration;
		std::map<double, double> _innerRanges;
		uint64_t _outerRangeDuration;
		std::map<double, double> _outerRanges;

		void Tick() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_ANIMATEDSPOTLIGHTCOMPONENT_H_ */

/**
 * @}
 */
