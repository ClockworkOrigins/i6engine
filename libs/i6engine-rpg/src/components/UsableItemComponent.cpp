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

#include "i6engine/rpg/components/UsableItemComponent.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	UsableItemComponent::UsableItemComponent(int64_t id, const api::attributeMap & params) : ItemComponent(id, params) {
		_objComponentID = ComponentTypes::UsableItemComponent;
	}

	api::ComPtr UsableItemComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<UsableItemComponent, api::Component>(id, params);
	}

	void UsableItemComponent::Init() {
	}

	api::attributeMap UsableItemComponent::synchronize() {
		api::attributeMap params = ItemComponent::synchronize();

		for (auto & p : _attributeChange) {
			params.insert(std::make_pair("attribute_" + std::to_string(int32_t(p.first)), std::to_string(p.second)));
		}

		return params;
	}

	bool UsableItemComponent::use(const api::GOPtr & self) {
		// TODO: (Daniel) add attribute change as soon as NPC template is available
		return true;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

REGISTERCOMPONENT(i6engine::rpg::components::UsableItemComponent, UsableItemComponent)
