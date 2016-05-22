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

#include "i6engine/api/components/MovementComponent.h"

#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/VelocityComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	MovementComponent::MovementComponent(const int64_t id, const attributeMap & params) : Component(id, params), _psc(), _vc() {
		_objFamilyID = components::ComponentTypes::MovementComponent;
	}

	MovementComponent::~MovementComponent() {
	}

	void MovementComponent::Init() {
		addTicker();
		_psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::ComponentTypes::PhysicalStateComponent);
		_vc = getOwnerGO()->getGOC<VelocityComponent>(components::ComponentTypes::VelocityComponent);
	}

	void MovementComponent::Tick() {
	}

	void MovementComponent::Finalize() {
		removeTicker();
	}

} /* namespace api */
} /* namespace i6e */
