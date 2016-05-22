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

#include "i6engine/rpg/components/NameComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6e {
namespace rpg {
namespace components {

	NameComponent::NameComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _name() {
		_objFamilyID = config::ComponentTypes::NameComponent;
		_objComponentID = config::ComponentTypes::NameComponent;

		api::parseAttribute<true>(params, "name", _name);
	}

	api::ComPtr NameComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<NameComponent, api::Component>(id, params);
	}

	void NameComponent::Init() {
	}

	api::attributeMap NameComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "name", _name);
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
