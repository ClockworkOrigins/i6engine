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

#include "i6engine/rpg/components/ExperienceComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6e {
namespace rpg {
namespace components {

	ExperienceComponent::ExperienceComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _currentXP(), _nextXP(), _level() {
		_objFamilyID = config::ComponentTypes::ExperienceComponent;
		_objComponentID = config::ComponentTypes::ExperienceComponent;

		api::parseAttribute<true>(params, "currentXP", _currentXP);
		api::parseAttribute<true>(params, "nextXP", _nextXP);
		api::parseAttribute<true>(params, "level", _level);
	}

	api::ComPtr ExperienceComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<ExperienceComponent, api::Component>(id, params);
	}

	void ExperienceComponent::Init() {
	}

	api::attributeMap ExperienceComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "currentXP", _currentXP);
		api::writeAttribute(params, "nextXP", _nextXP);
		api::writeAttribute(params, "level", _level);
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
