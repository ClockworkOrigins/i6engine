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

#include "i6engine/rpg/item/ItemManager.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/TextManager.h"

namespace i6e {
namespace rpg {
namespace item {

	ItemManager::ItemManager() : utils::Singleton<ItemManager>(), _parser() {
	}

	ItemManager::~ItemManager() {
	}

	void ItemManager::createItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot) {
		auto p = _parser.getTemplate(identifier);

		api::attributeMap paramsSSC;
		pos.insertInMap("pos", paramsSSC);
		rot.insertInMap("rot", paramsSSC);
		p.second._components.push_back(i6e::api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));

		api::EngineController::GetSingletonPtr()->getObjectFacade()->createObject(p.first, p.second, api::EngineController::GetSingletonPtr()->getUUID(), false);
	}

	std::string ItemManager::getItemName(const std::string & identifier) const {
		auto it = _parser._items.find(identifier);
		if (it == _parser._items.end()) {
			ISIXE_THROW_FAILURE("ItemManager", "Item with identifier '" << identifier << "' not found!");
		}
		for (auto c : it->second.second._components) {
			if (c._template == "Name") {
				return api::EngineController::GetSingleton().getTextManager()->getText(c._params["name"]);
			}
		}
		return "";
	}

} /* namespace item */
} /* namespace rpg */
} /* namespace i6e */
