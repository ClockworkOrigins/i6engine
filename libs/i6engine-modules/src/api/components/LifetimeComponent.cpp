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

#include "i6engine/api/components/LifetimeComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
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
} /* namespace i6engine */
