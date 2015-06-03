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
 * \addtogroup rpg
 * @{
 */

#ifndef __I6ENGINE_RPG_COMPONENTS_WEIGHTINVENTORYCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_WEIGHTINVENTORYCOMPONENT_H__

#include "i6engine/api/facades/MessageSubscriberFacade.h"

#include "i6engine/rpg/components/InventoryComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	class ISIXE_RPG_API WeightInventoryComponent : public InventoryComponent, public api::MessageSubscriberFacade {
	public:
		WeightInventoryComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		void Init() override;
		void Finalize() override;

		api::attributeMap synchronize() const override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr & comp) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const override {
			return "WeightInventory";
		}

		/**
		 * \brief checks whether the item can be added to the inventory and if so it is added
		 */
		bool addItem(const api::GOPtr & item) override;

		/**
		 * \brief shows the inventory, implementation depends on subclass
		 */
		void show() override;

		/**
		 * \brief hides the inventory, implementation depends on subclass
		 */
		void hide() override;

		/**
		 * \brief tries to use given item
		 */
		void useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback) override;

		/**
		 * \brief returns the selected item
		 * if none is selected, first parameter in pair is UINT32_MAX
		 */
		std::tuple<uint32_t, std::string, std::string, std::string> getSelectedItem() const override;

		/**
		 * \brief returns the number of items for the given type
		 */
		uint32_t getItemCount(const std::string & identifier) const override;
		uint32_t getItemCount(uint32_t item, const std::string & name) const override;

		/**
		 * \brief used to create an item in the inventory
		 */
		void removeItems(const std::string & identifier, uint32_t amount) override;

	private:
		enum class Filter {
			None,
			UsableItems
		};

		enum ItemEntry {
			Message,
			Amount,
			Imageset,
			Image,
			Identifier
		};

		std::map<uint32_t, std::map<std::string, std::tuple<api::GameMessage::Ptr, uint32_t, std::string, std::string, std::string>>> _items;
		double _maxWeight;
		double _currentWeight;
		uint32_t _currentIndex;
		uint32_t _maxShowIndex;
		Filter _currentFilter;
		uint32_t _slotsPerView;
		std::vector<std::string> _widgetList;

		void News(const api::GameMessage::Ptr & msg) override;
		void Tick() override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_WEIGHTINVENTORYCOMPONENT_H__ */

/**
 * @}
 */
