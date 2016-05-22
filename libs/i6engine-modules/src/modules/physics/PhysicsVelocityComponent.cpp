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

#include "i6engine/modules/physics/PhysicsVelocityComponent.h"

#include "i6engine/modules/physics/PhysicsNode.h"

namespace i6e {
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
} /* namespace i6e */
