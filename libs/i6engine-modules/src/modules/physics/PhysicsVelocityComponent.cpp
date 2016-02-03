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

#include "i6engine/modules/physics/PhysicsVelocityComponent.h"

#include "i6engine/modules/physics/PhysicsNode.h"

namespace i6engine {
namespace modules {

	PhysicsVelocityComponent::PhysicsVelocityComponent(PhysicsNode * node, double mass, double maxSpeed, double resistanceCoefficient, double windage) : _node(node), _mass(mass), _maxSpeed(maxSpeed), _resistanceCoefficient(resistanceCoefficient), _windage(windage), _acceleration(), _handling(api::VelocityComponent::MaxSpeedHandling::StopAcceleration), _callback(), _accelerating(false), _decelerating(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	void PhysicsVelocityComponent::accelerate(const Vec3 & acceleration, api::VelocityComponent::MaxSpeedHandling handling, const std::function<void(void)> & callback) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_acceleration = acceleration;
		_handling = handling;
		_callback = callback;

		if (_decelerating) {
			_node->removeTicker();
			_decelerating = false;
		}
		if (!_accelerating) {
			_node->addTicker();
			_accelerating = true;
		}
	}

	void PhysicsVelocityComponent::decelerate(const Vec3 & deceleration, api::VelocityComponent::DecelerationHandling handling, const std::function<void(void)> & callback) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_acceleration = deceleration;
		_decelerationHandling = handling;
		_callback = callback;

		if (_accelerating) {
			_node->removeTicker();
			_accelerating = false;
		}
		if (!_decelerating) {
			_node->addTicker();
			_decelerating = true;
		}
	}

	void PhysicsVelocityComponent::stopAcceleration() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_accelerating) {
			_node->removeTicker();
			_accelerating = false;
		}
		if (_decelerating) {
			_node->removeTicker();
			_decelerating = false;
		}
	}

	void PhysicsVelocityComponent::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_accelerating || _decelerating) {
			Vec3 F = _acceleration * _mass;
			Vec3 Fw = (_acceleration.normalize() * _node->getVelocity().length() * 1.2041 / 2) * _resistanceCoefficient * _windage;

			_node->applyForce(F - Fw, Vec3::ZERO, true);

			if (_accelerating && _node->getVelocity().length() >= _maxSpeed) {
				if (_handling == api::VelocityComponent::MaxSpeedHandling::StopAcceleration) {
					_accelerating = false;
					_node->removeTicker();
				}
				_callback();
			} else if (_decelerating && std::abs(_node->getVelocity().length()) < 1.0) {
				if (_decelerationHandling == api::VelocityComponent::DecelerationHandling::StopDeceleration) {
					_decelerating = false;
					_node->removeTicker();
				}
				_callback();
			}
		}
	}

} /* namespace modules */
} /* namespace i6engine */
