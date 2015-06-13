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

#include "i6engine/rpg/components/MiscItemComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	MiscItemComponent::MiscItemComponent(int64_t id, const api::attributeMap & params) : ItemComponent(id, params) {
		_objComponentID = config::ComponentTypes::MiscItemComponent;
	}

	api::ComPtr MiscItemComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<MiscItemComponent, api::Component>(id, params);
	}

	void MiscItemComponent::Init() {
	}

	api::attributeMap MiscItemComponent::synchronize() const {
		api::attributeMap params = ItemComponent::synchronize();
		return params;
	}

	std::vector<std::pair<std::string, std::string>> MiscItemComponent::getInfos() const {
		std::vector<std::pair<std::string, std::string>> vec;
		vec.push_back(std::make_pair("Weight_Key", utils::to_string_with_precision(getWeight(), 3)));
		return vec;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
