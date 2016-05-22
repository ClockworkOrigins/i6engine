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
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_PHYSICSVELOCITYCOMPONENT_H__
#define __I6ENGINE_MODULES_PHYSICSVELOCITYCOMPONENT_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/components/VelocityComponent.h"

namespace i6e {
namespace modules {

	class PhysicsNode;

	/**
	 * \class PhysicsVelocityComponent
	 * \brief Handles calculation of air friction
	 */
	class PhysicsVelocityComponent {
	public:
		PhysicsVelocityComponent(PhysicsNode * node, double mass, double maxSpeed, double resistanceCoefficient, double windage);

		void accelerate(const Vec3 & acceleration, api::VelocityComponent::MaxSpeedHandling handling, const std::function<void(void)> & callback);

		void decelerate(const Vec3 & deceleration, api::VelocityComponent::DecelerationHandling handling, const std::function<void(void)> & callback);

		void stopAcceleration();

		inline void setMaxSpeed(double maxSpeed) {
			_maxSpeed = maxSpeed;
		}

		inline void setResistanceCoefficient(double resistanceCoefficient) {
			_resistanceCoefficient = resistanceCoefficient;
		}

		inline void setWindage(double windage) {
			_windage = windage;
		}

		void Tick();

	private:
		PhysicsNode * _node;
		double _mass;
		double _maxSpeed;
		double _resistanceCoefficient;
		double _windage;
		Vec3 _acceleration;
		api::VelocityComponent::MaxSpeedHandling _handling;
		api::VelocityComponent::DecelerationHandling _decelerationHandling;
		std::function<void(void)> _callback;
		bool _accelerating;
		bool _decelerating;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PHYSICSVELOCITYCOMPONENT_H__ */

/**
 * @}
 */
