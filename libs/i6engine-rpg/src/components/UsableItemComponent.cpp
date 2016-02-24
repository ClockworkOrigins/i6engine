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

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/AttributeComponent.h"
#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	UsableItemComponent::UsableItemComponent(int64_t id, const api::attributeMap & params) : ItemComponent(id, params) {
		_objComponentID = config::ComponentTypes::UsableItemComponent;

		for (uint32_t i = 0; i < uint32_t(Attribute::COUNT); i++) {
			auto it = params.find("attribute_" + std::to_string(i));
			if (it != params.end()) {
				_attributeChange.insert(std::make_pair(Attribute(i), std::stoi(it->second)));
			}
		}
	}

	api::ComPtr UsableItemComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<UsableItemComponent, api::Component>(id, params);
	}

	void UsableItemComponent::Init() {
	}

	api::attributeMap UsableItemComponent::synchronize() const {
		api::attributeMap params = ItemComponent::synchronize();

		for (auto & p : _attributeChange) {
			api::writeAttribute(params, "attribute_" + std::to_string(int32_t(p.first)), p.second);
		}

		return params;
	}

	bool UsableItemComponent::use(const api::GOPtr & self) {
		auto ac = self->getGOC<AttributeComponent>(config::ComponentTypes::AttributeComponent);
		for (auto & p : _attributeChange) {
			ac->changeAttribute(p.first, p.second);
		}
		return true;
	}

	std::vector<std::pair<std::string, std::string>> UsableItemComponent::getInfos() const {
		std::vector<std::pair<std::string, std::string>> vec;
		for (auto & p : _attributeChange) {
			vec.push_back(std::make_pair("Attribute_" + std::to_string(uint32_t(p.first)) + "_Key", std::to_string(p.second)));
		}
		vec.push_back(std::make_pair("Weight_Key", utils::to_string_with_precision(getWeight(), 3)));
		return vec;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
