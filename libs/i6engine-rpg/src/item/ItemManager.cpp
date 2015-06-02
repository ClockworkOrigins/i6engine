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

#include "i6engine/rpg/item/ItemManager.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"

namespace i6engine {
namespace rpg {
namespace item {

	ItemManager::ItemManager() : _parser() {
	}

	ItemManager::~ItemManager() {
	}

	void ItemManager::createItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot) {
		auto p = _parser.getTemplate(identifier);

		api::attributeMap paramsSSC;
		pos.insertInMap("pos", paramsSSC);
		rot.insertInMap("rot", paramsSSC);
		p.second._components.push_back(i6engine::api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));

		api::EngineController::GetSingletonPtr()->getObjectFacade()->createObject(p.first, p.second, api::EngineController::GetSingletonPtr()->getUUID(), false);
	}

} /* namespace item */
} /* namespace rpg */
} /* namespace i6engine */
