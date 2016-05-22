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

#ifndef __I6ENGINE_API_FOLLOWCOMPONENT_H__
#define __I6ENGINE_API_FOLLOWCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	class PhysicalStateComponent;

	/**
	 * \brief Component for objects following another one (e.g. projectiles)
	 * For creating a FollowComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | targetID | yes | int64_t | id of the GameObject that should be followed | yes |
	 * | speed | yes | double | speed in meters / second | yes |
	 */
	class ISIXE_MODULES_API FollowComponent : public Component {
	public:
		FollowComponent(const int64_t id, const attributeMap & params);

		~FollowComponent();

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Follow";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

	private:
		utils::weakPtr<PhysicalStateComponent, Component> _ownPSC;
		utils::weakPtr<PhysicalStateComponent, Component> _targetPSC;

		uint64_t _lastTime;

		/**
		 * \brief speed in m/s
		 */
		double _speed;

		void Init() override;

		void Finalize() override;

		void Tick() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_FOLLOWCOMPONENT_H__ */

/**
 * @}
 */
