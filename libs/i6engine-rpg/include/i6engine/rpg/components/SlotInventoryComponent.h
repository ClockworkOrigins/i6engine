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

#ifndef __I6ENGINE_RPG_COMPONENTS_SLOTINVENTORYCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_SLOTINVENTORYCOMPONENT_H__

#include "i6engine/api/facades/MessageSubscriberFacade.h"

#include "i6engine/rpg/components/InventoryComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	class ISIXE_RPG_API SlotInventoryComponent : public InventoryComponent, public api::MessageSubscriberFacade {
	public:
		SlotInventoryComponent(int64_t id, const api::attributeMap & params);

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

		std::string getTemplateName() const {
			return "SlotInventory";
		}

		/**
		 * \brief checks whether the item can be added to the inventory and if so it is added
		 */
		bool addItem(const api::GOPtr & item);

		/**
		 * \brief shows the inventory, implementation depends on subclass
		 */
		void show();

		/**
		 * \brief hides the inventory, implementation depends on subclass
		 */
		void hide();

		/**
		 * \brief tries to use given item
		 */
		void useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback);

		/**
		 * \brief returns the selected item
		 * if none is selected, first parameter in pair is UINT32_MAX
		 */
		std::tuple<uint32_t, std::string, std::string, std::string> getSelectedItem() const;

		/**
		 * \brief returns the number of items for the given type
		 */
		uint32_t getItemCount(uint32_t item, const std::string & name) const;

	private:
		enum ItemEntry {
			Type,
			Name,
			Message,
			Imageset,
			Image,
			Width,
			Height,
			Infos
		};

		uint16_t _rows;
		uint16_t _columns;
		std::vector<std::string> _widgetList;
		bool _slotMarker;
		uint16_t _currentIndex;

		/**
		 * \brief contains for every slot the index of the item in list or UINT16_MAX, if empty
		 */
		std::vector<std::vector<uint16_t>> _slots;
		std::vector<std::tuple<uint32_t, std::string, api::GameMessage::Ptr, std::string, std::string, uint16_t, uint16_t, std::vector<std::pair<std::string, std::string>>>> _items;


		/**
		 * \brief tries to use given item
		 */
		void useItem(uint32_t item, const std::string & name, const std::function<void(void)> & callback, uint16_t index);

		void News(const api::GameMessage::Ptr & msg);
		void Tick() override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_SLOTINVENTORYCOMPONENT_H__ */

/**
 * @}
 */