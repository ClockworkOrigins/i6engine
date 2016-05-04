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
