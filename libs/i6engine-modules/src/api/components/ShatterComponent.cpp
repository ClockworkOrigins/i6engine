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

#include "i6engine/api/components/ShatterComponent.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	ShatterComponent::ShatterComponent(const int64_t id, const attributeMap & params) : Component(id, params), _respawn(0) {
		Component::_objFamilyID = components::ShatterComponent;
		Component::_objComponentID = components::ShatterComponent;
	}

	ShatterComponent::~ShatterComponent() {
	}

	void ShatterComponent::Init() {
	}

	void ShatterComponent::News(const GameMessage::Ptr & msg) {
		uint16_t type = msg->getSubtype();

		if (type == components::ComShatter) {
			int64_t b = static_cast<components::Component_Shatter_Update *>(msg->getContent())->other;
			GOPtr go = i6eObjectFacade->getObject(b);

			if (go == nullptr) {
				return;
			}

#ifdef ISIXE_WITH_PROFILING
			msg->insertTimestamp("ShatterComponent: " + _objComponentID);
#endif /* ISIXE_WITH_PROFILING */

			shatter(go);
		} else {
			Component::News(msg);
		}
	}

	attributeMap ShatterComponent::synchronize() const {
		return attributeMap();
	}

} /* namespace api */
} /* namespace i6e */
