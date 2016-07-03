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
	 * | scale | no | Vec3 | relative scale to SceneNode | yes |
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
		 * \brief sets the scale of this particle relative to its SceneNode
		 */
		void setScale(const Vec3 & scale);

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
		Vec3 _scale;
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
