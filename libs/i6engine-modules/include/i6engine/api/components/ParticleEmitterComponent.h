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

#ifndef __I6ENGINE_API_PARTICLEEMITTERCOMPONENT_H__
#define __I6ENGINE_API_PARTICLEEMITTERCOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class ParticleEmitterComponent
	 * \brief Emits Particles
	 * For creating a ParticleEmitterComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | particleEmitter | yes | std::string | The emitter for the particle | yes |
	 * | pos | no | Vec3 | relative position to SceneNode | yes |
	 * | fadeOut | no | bool | if set to true, the particle isn't removed immediately but emitters are stopped and so the particle fades out | yes |
	 * | fadeOutCooldown | *) | uint64_t | time after stopping emitters until particle will be destroyed, *) required if fadeOut is set | yes |
	 */
	class ISIXE_MODULES_API ParticleEmitterComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component
		 *
		 * Sets visibility and ID
		 */
		ParticleEmitterComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Empty
		 *
		 * Hasn't to do anything right now
		 */
		~ParticleEmitterComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "ParticleEmitter";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		std::string _emitterName;
		Vec3 _pos;
		bool _fadeOut;
		uint32_t _fadeOutCooldown;

		void Init() override;

		void Finalize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_PARTICLEEMITTERCOMPONENT_H__ */

/**
 * @}
 */
