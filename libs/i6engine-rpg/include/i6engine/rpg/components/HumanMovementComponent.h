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
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_COMPONENTS_HUMANMOVEMENTCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_HUMANMOVEMENTCOMPONENT_H__

#include "i6engine/api/components/MovementComponent.h"

namespace i6e {
namespace rpg {
namespace components {

	/**
	 * \brief movement component for humans
	 */
	class ISIXE_RPG_API HumanMovementComponent : public api::MovementComponent {
	public:
		HumanMovementComponent(const int64_t id, const api::attributeMap & params);
		virtual ~HumanMovementComponent();

		static api::ComPtr createC(const int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "HumanMovement";
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		void forward() override;

		void backward() override;

		void left() override;

		void right() override;

		void stop() override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_HUMANMOVEMENTCOMPONENT_H__ */

/**
 * @}
 */
