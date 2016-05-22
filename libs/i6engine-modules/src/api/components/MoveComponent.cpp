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

#include "i6engine/api/components/MoveComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/components/MovementComponent.h"
#include "i6engine/api/components/NavigationComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	MoveComponent::MoveComponent(const int64_t id, const attributeMap & params) : Component(id, params), _mc(), _index(0), _path() {
		_objFamilyID = components::ComponentTypes::MoveComponent;
		_objComponentID = components::ComponentTypes::MoveComponent;
	}

	MoveComponent::~MoveComponent() {
	}

	void MoveComponent::Init() {
		addTicker();
		_mc = getOwnerGO()->getGOC<MovementComponent>(components::ComponentTypes::MovementComponent);
	}

	void MoveComponent::Tick() {
		if (_index < _path.size()) {
			auto psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
			if ((psc->getPosition() - _path[_index]).length() < 0.5) {
				_index++;
				if (_index == _path.size()) {
					auto mc = _mc.get();
					mc->stop();
					return;
				}
			}
			double angle = Vec3::crossAngleSigned(math::rotateVector(Vec3(0.0, 0.0, -1.0), psc->getRotation()), _path[_index] - psc->getPosition(), Vec3(0, 1, 0));
			auto mc = _mc.get();
			if (angle > 0) {
				mc->left();
			} else if (angle < 0) {
				mc->right();
			}
			if (std::abs(angle) < PI / 4.0) { // only move forward if the new target point is in a 45 degree range, otherwise only turn to the side just in place
				mc->forward();
			}
		}
	}

	void MoveComponent::Finalize() {
		removeTicker();
	}

	attributeMap MoveComponent::synchronize() const {
		return attributeMap();
	}

	void MoveComponent::navigate(const Vec3 & to) {
		auto psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
		_path = getOwnerGO()->getGOC<NavigationComponent>(components::ComponentTypes::NavigationComponent)->getPath(psc->getPosition(), to);
		_index = 0;
	}

	void MoveComponent::navigate(const std::string & to) {
		auto psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
		_path = getOwnerGO()->getGOC<NavigationComponent>(components::ComponentTypes::NavigationComponent)->getPath(psc->getPosition(), to);
		_index = 0;
	}

} /* namespace api */
} /* namespace i6e */
