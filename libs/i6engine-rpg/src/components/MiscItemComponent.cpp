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

#include "i6engine/rpg/components/MiscItemComponent.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/rpg/components/Config.h"

namespace i6e {
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
} /* namespace i6e */
