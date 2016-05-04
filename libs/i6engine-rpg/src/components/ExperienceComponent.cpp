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

namespace i6e {
namespace rpg {
namespace components {

	ExperienceComponent::ExperienceComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _currentXP(), _nextXP(), _level() {
		_objFamilyID = config::ComponentTypes::ExperienceComponent;
		_objComponentID = config::ComponentTypes::ExperienceComponent;

		api::parseAttribute<true>(params, "currentXP", _currentXP);
		api::parseAttribute<true>(params, "nextXP", _nextXP);
		api::parseAttribute<true>(params, "level", _level);
	}

	api::ComPtr ExperienceComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<ExperienceComponent, api::Component>(id, params);
	}

	void ExperienceComponent::Init() {
	}

	api::attributeMap ExperienceComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "currentXP", _currentXP);
		api::writeAttribute(params, "nextXP", _nextXP);
		api::writeAttribute(params, "level", _level);
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
