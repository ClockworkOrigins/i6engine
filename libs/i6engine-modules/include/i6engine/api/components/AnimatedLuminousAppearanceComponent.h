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

#ifndef __I6ENGINE_API_ANIMATEDLUMINOUSAPPEARANCECOMPONENT_H_
#define __I6ENGINE_API_ANIMATEDLUMINOUSAPPEARANCECOMPONENT_H_

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/LuminousAppearanceComponent.h"

namespace i6e {
namespace api {

	/**
	 * \class AnimatedLuminousAppearanceComponent
	 * \brief interpolates light colours and attenuation (e.g. range)
	 * For creating a AnimatedLuminousAppearanceComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | diffuseDuration | no | int | duration of one diffuse interpolation cycle | yes |
	 * | diffuseKey_<timePoint> | no | Vec3 | diffuse colour at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 * | specularDuration | no | int | duration of one specular interpolation cycle | yes |
	 * | specularKey_<timePoint> | no | Vec3 | specular colour at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 * | attenuationDuration | no | int | duration of one attenuation interpolation cycle | yes |
	 * | attenuationKey_<timePoint> | no | Vec4 | attenuation at timePoint encoded in name (timePoint between 0 and 1) | yes |
	 */
	class ISIXE_MODULES_API AnimatedLuminousAppearanceComponent : public LuminousAppearanceComponent {
	public:
		/**
		 * \brief Constructor sets ID and component
		 */
		AnimatedLuminousAppearanceComponent(const int64_t id, const attributeMap & params);

		virtual ~AnimatedLuminousAppearanceComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "AnimatedLuminousAppearance";
		}

		virtual std::vector<componentOptions> getComponentOptions() override;

	protected:
		virtual void Tick() override;

		/**
		 * \brief used to interpolate between two following values depending on current time
		 */
		template<typename T>
		T interpolate(uint64_t duration, const std::map<double, T> & values) const {
			uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
			double timePoint = (((cT - _startTime) % duration) + 1) / double(duration);
			for (auto it = values.begin(); it != values.end(); it++) {
				if (it->first <= timePoint) {
					double currentTimePoint = it->first;
					T currentValue = it->second;
					it++;
					double timeDiff = 0.0;
					double timePercent = 0.0;
					if (it == values.end()) {
						timeDiff = 1.0 - currentTimePoint;
						it = values.begin();
						timeDiff += it->first;
						timePercent = (timePoint + 1.0 - currentTimePoint) / timeDiff;
					} else {
						timeDiff = it->first - currentTimePoint;
						timePercent = (timePoint - currentTimePoint) / timeDiff;
					}
					T newValue = currentValue * (1.0 - timePercent) + (it->second) * timePercent;
					return newValue;
				}
			}
			ISIXE_THROW_FAILURE("AnimatedLuminousAppearanceComponent", "interpolation didn't work.");
			return T();
		}

	private:
		uint64_t _startTime;
		uint64_t _diffuseDuration;
		std::map<double, Vec3> _diffuseColours;
		uint64_t _specularDuration;
		std::map<double, Vec3> _specularColours;
		uint64_t _attenuationDuration;
		std::map<double, Vec4> _attenuations;

		void Init() override;
		void Finalize() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_ANIMATEDLUMINOUSAPPEARANCECOMPONENT_H_ */

/**
 * @}
 */
