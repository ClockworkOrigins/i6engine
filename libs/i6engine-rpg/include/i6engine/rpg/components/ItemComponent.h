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

#ifndef __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace rpg {
namespace components {

	/**
	 * \brief base class for all items
	 * derive from it and implement virtual methods to create own behavior
	 */
	class ISIXE_RPG_API ItemComponent : public api::Component {
	public:
		ItemComponent(int64_t id, const api::attributeMap & params);

		virtual api::attributeMap synchronize() const override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief self is using this item, return value decides whether item is removed from inventory or not
		 */
		virtual bool use(const api::GOPtr & self) = 0;

		uint32_t getValue() const {
			return _value;
		}

		std::string getImageset() const {
			return _imageset;
		}

		std::string getImage() const {
			return _image;
		}

		double getWeight() const {
			return _weight;
		}

		std::string getItemIdentifier() const {
			return _identifier;
		}

		/**
		 * \brief only used by SlotInventory to determine whether items of same type only take one slot or more
		 */
		bool isStackable() const {
			return _stackable;
		}

		/**
		 * \brief returns key value pairs for info screen for this item in inventory
		 */
		virtual std::vector<std::pair<std::string, std::string>> getInfos() const = 0;

	private:
		uint32_t _value;
		std::string _imageset;
		std::string _image;
		double _weight;
		std::string _identifier;
		bool _stackable;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__ */

/**
 * @}
 */
