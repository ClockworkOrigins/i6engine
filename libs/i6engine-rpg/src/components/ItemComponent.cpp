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

#include "i6engine/rpg/components/ItemComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	ItemComponent::ItemComponent(int64_t id, const api::attributeMap & params) : Component(id, params) {
		ISIXE_THROW_API_COND("ItemComponent", "value not set!", params.find("value") != params.end());
		_value = static_cast<uint32_t>(std::stoi(params.find("value")->second.c_str()));

		_objFamilyID = config::ComponentTypes::ItemComponent;
	}

	api::attributeMap ItemComponent::synchronize() {
		api::attributeMap params;

		params.insert(std::make_pair("value", std::to_string(_value)));

		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
