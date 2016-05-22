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

#include "i6engine/rpg/components/AttributeComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6e {
namespace rpg {
namespace components {

	AttributeComponent::AttributeComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _attributes(size_t(Attribute::COUNT)) {
		_objFamilyID = config::ComponentTypes::AttributeComponent;
		_objComponentID = config::ComponentTypes::AttributeComponent;

		int32_t attribute0 = 0;
		int32_t attribute1 = 0;
		api::parseAttribute<true>(params, "attribute_0", attribute0);
		api::parseAttribute<true>(params, "attribute_1", attribute1);
		_attributes[0] = attribute0;
		_attributes[1] = attribute1;
	}

	api::ComPtr AttributeComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<AttributeComponent, api::Component>(id, params);
	}

	void AttributeComponent::Init() {
	}

	api::attributeMap AttributeComponent::synchronize() const {
		api::attributeMap params;

		for (size_t i = 0; i < _attributes.size(); i++) {
			api::writeAttribute<int32_t>(params, "attribute_" + std::to_string(i), _attributes[i]);
		}

		return params;
	}

	void AttributeComponent::changeAttribute(Attribute attribute, int32_t diff) {
		_attributes[size_t(attribute)] = _attributes[size_t(attribute)] + diff;
	}

	void AttributeComponent::registerListener(Attribute attribute, const std::function<void(int32_t)> & func) {
		_attributes[size_t(attribute)].registerUpdate(func);
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
