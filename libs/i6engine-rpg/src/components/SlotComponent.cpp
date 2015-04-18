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

#include "i6engine/rpg/components/SlotComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	SlotComponent::SlotComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _width(std::stoul(params.find("width")->second)), _height(std::stoul(params.find("height")->second)) {
		_objFamilyID = config::ComponentTypes::SlotComponent;
		_objComponentID = config::ComponentTypes::SlotComponent;
	}

	api::ComPtr SlotComponent::createC(int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("SlotComponent", "width not set!", params.find("width") != params.end());
		ISIXE_THROW_API_COND("SlotComponent", "height not set!", params.find("height") != params.end());
		return utils::make_shared<SlotComponent, api::Component>(id, params);
	}

	void SlotComponent::Init() {
	}

	api::attributeMap SlotComponent::synchronize() const {
		api::attributeMap params;

		params.insert(std::make_pair("width", std::to_string(_width)));
		params.insert(std::make_pair("height", std::to_string(_height)));

		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
