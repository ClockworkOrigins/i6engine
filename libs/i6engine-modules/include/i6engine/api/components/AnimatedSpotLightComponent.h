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

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_ANIMATEDSPOTLIGHTCOMPONENT_H_
#define __I6ENGINE_API_ANIMATEDSPOTLIGHTCOMPONENT_H_

#include "i6engine/api/components/AnimatedLuminousAppearanceComponent.h"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_API_ANIMATEDSPOTLIGHTCOMPONENT_H_ */

/**
 * @}
 */
