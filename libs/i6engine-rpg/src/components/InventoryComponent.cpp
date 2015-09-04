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
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"

#include "i6engine/rpg/config/ExternalConstants.h"

#include "i6engine/rpg/item/ItemManager.h"

#ifdef max
	#undef max
#endif

namespace i6engine {
namespace rpg {
namespace components {

	InventoryComponent::InventoryComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _shown(false), _callbacks(), _trading(false), _isSelfInventory(true), _multiplier(1.0), _otherTradeInventory() {
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

			uint32_t oldAmount = getItemCount(identifier);

			api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO(p.first, p.second, api::EngineController::GetSingletonPtr()->getUUID(), false, [this, oldAmount, amount, identifier](api::GOPtr go) {
				if (addItem(go)) {
					if (getItemCount(identifier) == oldAmount + amount && _shown) {
						hide();
						show();
					}
					go->setDie();
				}
			});
		}
	}

	void InventoryComponent::startTrade(const utils::sharedPtr<InventoryComponent, api::Component> & otherInventory, double selfMultiplier, double otherMultiplier) {
		_isSelfInventory = true;
		otherInventory->_isSelfInventory = false;
		_trading = true;
		otherInventory->_trading = true;

		_multiplier = selfMultiplier;
		otherInventory->_multiplier = otherMultiplier;

		_otherTradeInventory = otherInventory;
		otherInventory->_otherTradeInventory = utils::dynamic_pointer_cast<InventoryComponent>(_self.get());

		showTradeView(otherInventory);
		otherInventory->showTradeView(utils::dynamic_pointer_cast<InventoryComponent>(_self.get()));
	}

	void InventoryComponent::tradeItem(const std::string & identifier, uint64_t value) {
		// called from players inventory, so this is player
		auto inventory = _otherTradeInventory.get();
		if (inventory->getItemCount(rpg::config::CURRENCY) >= std::max(1u, uint32_t(value * _multiplier)) || (_isSelfInventory && !rpg::config::LIMITED_TRADER_GOLD)) {
			inventory->createItems(identifier, 1);
			removeItems(identifier, 1);
			if (_isSelfInventory) {
				if (rpg::config::LIMITED_TRADER_GOLD) {
					inventory->removeItems(rpg::config::CURRENCY, std::max(1u, uint32_t(value * _multiplier)));
				}
			} else {
				inventory->removeItems(rpg::config::CURRENCY, std::max(1u, uint32_t(value * _multiplier)));
			}
			createItems(rpg::config::CURRENCY, std::max(1u, uint32_t(value * _multiplier)));
		}
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
