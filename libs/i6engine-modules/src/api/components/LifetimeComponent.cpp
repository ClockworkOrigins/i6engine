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

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	LifetimeComponent::LifetimeComponent(const int64_t id, const attributeMap & params) : Component(id, params), _startTime(EngineController::GetSingleton().getCurrentTime()), _lifetime(0) {
		Component::_objFamilyID = components::LifetimeComponent;;
		Component::_objComponentID = components::LifetimeComponent;

		_lifetime = boost::lexical_cast<uint64_t>(params.find("lifetime")->second);

		std::cout << "Lifetime: " << _lifetime << std::endl;
	}

	ComPtr LifetimeComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("LifetimeComponent", "lifetime not set!", params.find("lifetime") != params.end());
		return utils::make_shared<LifetimeComponent, Component>(id, params);
	}

	LifetimeComponent::~LifetimeComponent() {
		removeTicker();
	}

	void LifetimeComponent::Init() {
		addTicker();
	}

	void LifetimeComponent::Tick() {
		uint64_t cT = EngineController::GetSingleton().getCurrentTime();

		if (_startTime + _lifetime <= cT) {
			dead();
		}
	}

	void LifetimeComponent::dead() {
		getOwnerGO()->setDie();
	}

	void LifetimeComponent::instantKill() {
		_lifetime = 0;
	}

	attributeMap LifetimeComponent::synchronize() {
		attributeMap params;
		params["lifetime"] = boost::lexical_cast<std::string>(_lifetime - _startTime);
		return params;
	}

	std::vector<componentOptions> LifetimeComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Lifetime", [this]() {
			return boost::lexical_cast<std::string>(_lifetime);
		}, [this](std::string s) {
			_lifetime = boost::lexical_cast<uint64_t>(s);
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */

REGISTERCOMPONENT(i6engine::api::LifetimeComponent, LifetimeComponent)
