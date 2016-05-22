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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_COMPONENTS_LISTINVENTORYCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_LISTINVENTORYCOMPONENT_H__

#include "i6engine/api/facades/MessageSubscriberFacade.h"

#include "i6engine/rpg/components/InventoryComponent.h"

namespace i6e {
namespace rpg {
namespace components {

	/**
	 * \brief inventory sorting items depending on their type listing all in one view
	 */
	class ISIXE_RPG_API ListInventoryComponent : public InventoryComponent, public api::MessageSubscriberFacade {
	public:
		ListInventoryComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const override {
			return "ListInventory";
		}

	private:
		enum ItemEntry {
			Message,
			Amount,
			Imageset,
			Image,
			Identifier,
			Value
		};
		std::map<uint32_t, std::map<std::string, std::tuple<api::GameMessage::Ptr, uint32_t, std::string, std::string, std::string, uint32_t>>> _items;
		uint32_t _columns;
		uint32_t _slotCount;
		uint32_t _currentIndex;
		std::vector<std::string> _widgets;
		uint32_t _itemTypeCount;
		uint32_t _maxSlot;
		bool _active;
		bool _infoScreen;

		void Init() override;
		void Finalize() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
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

		void showItems();

		void News(const api::GameMessage::Ptr & msg) override;
		void Tick() override;

		void showTradeView(const utils::sharedPtr<InventoryComponent, api::Component> & otherInventory) override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_LISTINVENTORYCOMPONENT_H__ */

/**
 * @}
 */
