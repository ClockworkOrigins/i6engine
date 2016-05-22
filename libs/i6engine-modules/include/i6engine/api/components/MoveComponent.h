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

#ifndef __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__
#define __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	class MovementComponent;

	/**
	 * \brief Component used for moving through the world
	 * requires both, a MovementComponent and a NavigationComponent to know how to move and get the route
	 */
	class ISIXE_MODULES_API MoveComponent : public Component {
	public:
		MoveComponent(const int64_t id, const attributeMap & params);
		~MoveComponent();

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Move";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief moves GameObject from current position to target position
		 */
		void navigate(const Vec3 & to);
		
		/**
		 * \brief moves GameObject from current position to target waypoint
		 */
		void navigate(const std::string & to);

	private:
		utils::weakPtr<MovementComponent, Component> _mc;
		uint16_t _index;
		std::vector<Vec3> _path;

		void Init() override;

		void Tick() override;

		void Finalize() override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_COMPONENTS_MOVECOMPONENT_H__ */

/**
 * @}
 */
