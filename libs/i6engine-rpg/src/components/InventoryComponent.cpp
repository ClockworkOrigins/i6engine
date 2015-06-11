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

#include "i6engine/rpg/components/InventoryComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/rpg/components/Config.h"

#include "i6engine/rpg/item/ItemManager.h"

namespace i6engine {
namespace rpg {
namespace components {

	InventoryComponent::InventoryComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _shown(false), _callbacks(), _trading(false) {
		_objFamilyID = config::ComponentTypes::InventoryComponent;
	}

	void InventoryComponent::Init() {
	}

	api::attributeMap InventoryComponent::synchronize() const {
		api::attributeMap params;
		return params;
	}

	void InventoryComponent::createItems(const std::string & identifier, uint32_t amount) {
		for (uint32_t i = 0; i < amount; i++) {
			auto p = item::ItemManager::GetSingleton().getTemplate(identifier);
			api::attributeMap paramsSSC;
			paramsSSC.insert(std::make_pair("pos", "0.0 0.0 0.0"));
			paramsSSC.insert(std::make_pair("rot", "1.0 0.0 0.0 0.0"));
			p.second._components.push_back(i6engine::api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));

			api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO(p.first, p.second, api::EngineController::GetSingletonPtr()->getUUID(), false, [this](api::GOPtr go) {
				addItem(go);
			});
		}
	}

	void InventoryComponent::startTrade(const utils::sharedPtr<InventoryComponent, api::Component> & otherInventory, double selfMultiplier, double otherMultiplier) {
		showTradeView(false);
		otherInventory->showTradeView(true);
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
