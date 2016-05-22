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

#include "i6engine/rpg/components/SlotComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6e {
namespace rpg {
namespace components {

	SlotComponent::SlotComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _width(), _height() {
		_objFamilyID = config::ComponentTypes::SlotComponent;
		_objComponentID = config::ComponentTypes::SlotComponent;

		api::parseAttribute<true>(params, "width", _width);
		api::parseAttribute<true>(params, "height", _height);
	}

	api::ComPtr SlotComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<SlotComponent, api::Component>(id, params);
	}

	void SlotComponent::Init() {
	}

	api::attributeMap SlotComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "width", _width);
		api::writeAttribute(params, "height", _height);
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
