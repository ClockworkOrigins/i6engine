/*
 * Copyright 2011-2012 Prof. Dr. Richard Lenz, Thomas Fischer
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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_COMPONENTS_INVENTORYCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_INVENTORYCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace rpg {
namespace components {

	/**
	 * \brief base class for inventory implementations
	 */
	class ISIXE_RPG_API InventoryComponent : public api::Component {
	public:
		InventoryComponent(int64_t id, const api::attributeMap & params);

		virtual ~InventoryComponent() {
		}

		api::attributeMap synchronize() const override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief checks whether the item can be added to the inventory and if so it is added
		 */
		virtual bool addItem(const api::GOPtr & item) = 0;

		/**
		 * \brief shows the inventory, implementation depends on subclass
		 */
		virtual void show() = 0;

		/**
		 * \brief hides the inventory, implementation depends on subclass
		 */
		virtual void hide() = 0;

		/**
		 * \brief returns whether the inventory is shown or not
		 */
		bool isActive() const {
			return _shown;
		}

		/**
		 * \brief returns true if shown AND in trading mode
		 */
		bool isTrading() const {
			return _shown && _trading;
		}

		/**
		 * \brief tries to use given item
		 */
		virtual void useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback) = 0;

		/**
		 * \brief returns the number of items for the given type
		 */
		virtual uint32_t getItemCount(const std::string & identifier) const = 0;

		/**
		 * \brief returns the number of items for the given type
		 */
		virtual uint32_t getItemCount(uint32_t item, const std::string & name) const = 0;

		/**
		 * \brief returns the selected item
		 * if none is selected, first parameter in tuple is UINT32_MAX
		 */
		virtual std::tuple<uint32_t, std::string, std::string, std::string> getSelectedItem() const = 0;

		/**
		 * \brief used to create an item in the inventory
		 */
		void createItems(const std::string & identifier, uint32_t amount);

		/**
		 * \brief used to create an item in the inventory
		 */
		virtual void removeItems(const std::string & identifier, uint32_t amount) = 0;

		/**
		 * \brief registers a callback to be called on every change on the item count
		 */
		void registerUpdateCallback(const std::function<void(uint32_t, const std::string &, uint32_t)> & callback) {
			_callbacks.push_back(callback);
		}

		/**
		 * \brief starts trading with other inventory
		 * \param[in] otherInventory inventory of the other NPC to trade with
		 * \param[in] selfMultiplier multiplier for the value player gets when selling item (1.0 = full value, 0.1 = 10 percent of value)
		 * \param[in] otherMultiplier multiplier for the value player has to pay buying item (1.0 = full value, 0.1 = 10 percent of value)
		 */
		void startTrade(const utils::sharedPtr<InventoryComponent, api::Component> & otherInventory, double selfMultiplier, double otherMultiplier);

	protected:
		bool _shown;
		std::vector<std::function<void(uint32_t, const std::string &, uint32_t)>> _callbacks;
		bool _trading;
		bool _isSelfInventory;
		double _multiplier;
		utils::weakPtr<InventoryComponent, api::Component> _otherTradeInventory;

		void Init() override;

		virtual void showTradeView(const utils::sharedPtr<InventoryComponent, api::Component> & otherInventory) = 0;

		void tradeItem(const std::string & identifier, uint64_t value);
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_INVENTORYCOMPONENT_H__ */

/**
 * @}
 */
