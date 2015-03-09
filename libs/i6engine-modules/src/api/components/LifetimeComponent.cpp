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

	LifetimeComponent::LifetimeComponent(const int64_t id, const attributeMap & params) : Component(id, params), _ticksToDie(0) {
		Component::_objFamilyID = components::LifetimeComponent;;
		Component::_objComponentID = components::LifetimeComponent;

		_ticksToDie = uint64_t(boost::lexical_cast<double>(params.find("lifetime")->second) / (EngineController::GetSingletonPtr()->getObjectFacade()->getFrameTime() / 1000));
	}

	ComPtr LifetimeComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("LifetimeComponent", "lifetime not set!", params.find("lifetime") != params.end());
		return utils::make_shared<LifetimeComponent, Component>(id, params);
	}

	LifetimeComponent::~LifetimeComponent() {
		removeTicker();
	}

	void LifetimeComponent::Init() {
		if (_ticksToDie > 0) {
			addTicker();
		}
	}

	void LifetimeComponent::Tick() {
		_ticksToDie--;

		if (_ticksToDie == 0) {
			dead();
		}
	}

	void LifetimeComponent::dead() {
		getOwnerGO().get()->setDie();
	}

	void LifetimeComponent::instantKill() {
		_ticksToDie = 1;
	}

	attributeMap LifetimeComponent::synchronize() {
		attributeMap params;
		params["lifetime"] = boost::lexical_cast<std::string>((EngineController::GetSingletonPtr()->getObjectFacade()->getFrameTime() / 1000) * _ticksToDie);
		return params;
	}

	std::vector<componentOptions> LifetimeComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Lifetime", [this]() {
			return boost::lexical_cast<std::string>((EngineController::GetSingletonPtr()->getObjectFacade()->getFrameTime() / 1000) * _ticksToDie);
		}, [this](std::string s) {
			_ticksToDie = uint64_t(boost::lexical_cast<double>(s) / (EngineController::GetSingletonPtr()->getObjectFacade()->getFrameTime() / 1000));
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */

REGISTERCOMPONENT(i6engine::api::LifetimeComponent, LifetimeComponent)
