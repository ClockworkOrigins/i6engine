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

#include "i6engine/rpg/components/ExperienceComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	ExperienceComponent::ExperienceComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _currentXP(), _nextXP(), _level() {
		_objFamilyID = config::ComponentTypes::ExperienceComponent;
		_objComponentID = config::ComponentTypes::ExperienceComponent;

		_currentXP = std::stoul(params.find("currentXP")->second);
		_nextXP = std::stoul(params.find("nextXP")->second);
		_level = std::stoul(params.find("level")->second);
	}

	api::ComPtr ExperienceComponent::createC(int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("ExperienceComponent", "currentXP not set!", params.find("currentXP") != params.end());
		ISIXE_THROW_API_COND("ExperienceComponent", "nextXP not set!", params.find("nextXP") != params.end());
		ISIXE_THROW_API_COND("ExperienceComponent", "level not set!", params.find("level") != params.end());
		return utils::make_shared<ExperienceComponent, api::Component>(id, params);
	}

	void ExperienceComponent::Init() {
	}

	api::attributeMap ExperienceComponent::synchronize() const {
		api::attributeMap params;

		params.insert(std::make_pair("currentXP", std::to_string(_currentXP)));
		params.insert(std::make_pair("nextXP", std::to_string(_nextXP)));
		params.insert(std::make_pair("level", std::to_string(_level)));

		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
