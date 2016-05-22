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

namespace i6e {
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
			p.second._components.push_back(i6e::api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));

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
} /* namespace i6e */
