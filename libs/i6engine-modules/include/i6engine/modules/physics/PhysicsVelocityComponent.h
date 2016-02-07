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
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_PHYSICSVELOCITYCOMPONENT_H__
#define __I6ENGINE_MODULES_PHYSICSVELOCITYCOMPONENT_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/components/VelocityComponent.h"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_PHYSICSVELOCITYCOMPONENT_H__ */

/**
 * @}
 */
