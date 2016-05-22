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

#include "i6engine/api/components/LifetimeComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	LifetimeComponent::LifetimeComponent(const int64_t id, const attributeMap & params) : Component(id, params), _startTime(EngineController::GetSingleton().getCurrentTime()), _lifetime(0) {
		_objFamilyID = components::LifetimeComponent;;
		_objComponentID = components::LifetimeComponent;

		parseAttribute<true>(params, "lifetime", _lifetime);
	}

	LifetimeComponent::~LifetimeComponent() {
	}

	void LifetimeComponent::Init() {
		addTicker();
	}

	void LifetimeComponent::Finalize() {
		removeTicker();
	}

	void LifetimeComponent::Tick() {
		uint64_t cT = EngineController::GetSingleton().getCurrentTime();

		if (_startTime + _lifetime <= cT) {
			dead();
		}
	}

	void LifetimeComponent::dead() {
		auto go = getOwnerGO();
		if (go != nullptr) {
			go->setDie();
		}
	}

	void LifetimeComponent::instantKill() {
		_lifetime = 0;
	}

	attributeMap LifetimeComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "lifetime", _lifetime - (EngineController::GetSingleton().getCurrentTime() - _startTime));
		return params;
	}

	std::vector<componentOptions> LifetimeComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Lifetime", [this]() {
			return std::to_string(_lifetime);
		}, [this](std::string s) {
			_lifetime = std::stoull(s);
			return true;
		}, "Integer"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
