/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/rpg/components/UsableItemComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/AttributeComponent.h"
#include "i6engine/rpg/components/Config.h"

namespace i6e {
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
} /* namespace i6e */
