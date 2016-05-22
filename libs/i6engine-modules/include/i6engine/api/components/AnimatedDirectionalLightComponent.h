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

#ifndef __I6ENGINE_API_ANIMATEDDIRECTIONALLIGHTCOMPONENT_H_
#define __I6ENGINE_API_ANIMATEDDIRECTIONALLIGHTCOMPONENT_H_

#include "i6engine/api/components/AnimatedLuminousAppearanceComponent.h"

namespace i6e {
namespace api {

	/**
	 * \class AnimatedDirectionalLightComponent
	 * \brief Tells the engine that this GameObject is emitting light.
	 * For creating a AnimatedDirectionalLightComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | directionDuration | no | int | duration of one direction interpolation cycle | yes |
	 * | directionKey_<timePoint> | no | Vec3 | direction at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 */
	class ISIXE_MODULES_API AnimatedDirectionalLightComponent : public AnimatedLuminousAppearanceComponent {
	public:
		/**
		 * \brief Constructor sets ID and component
		 */
		AnimatedDirectionalLightComponent(const int64_t id, const attributeMap & params);

		~AnimatedDirectionalLightComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "AnimatedDirectionalLight";
		}

	private:
		uint64_t _directionDuration;
		std::map<double, Vec3> _directions;

		void Tick() override;
		std::vector<componentOptions> getComponentOptions() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_ANIMATEDDIRECTIONALLIGHTCOMPONENT_H_ */

/**
 * @}
 */
