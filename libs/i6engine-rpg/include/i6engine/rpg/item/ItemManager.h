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

#ifndef __I6ENGINE_RPG_ITEM_ITEMMANAGER_H__
#define __I6ENGINE_RPG_ITEM_ITEMMANAGER_H__

#include <set>

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/rpg/item/ItemParser.h"

namespace i6e {
namespace rpg {
namespace item {

	/**
	 * \brief manages all item related stuff
	 */
	class ISIXE_RPG_API ItemManager : public utils::Singleton<ItemManager> {
		friend class utils::Singleton<ItemManager>;

	public:
		~ItemManager();

		/**
		 * \brief loads all item templates in given directory and subdirectories
		 */
		void loadItems(const std::string & directory) {
			_parser.loadItems(directory);
		}

		/**
		 * \brief creates item with given identifier on position with rotation
		 */
		void createItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot);

		std::pair<std::string, i6e::api::objects::GOTemplate> getTemplate(const std::string & identifier) const {
			return _parser.getTemplate(identifier);
		}

		/**
		 * \brief returns the item name for the given identifier
		 */
		std::string getItemName(const std::string & identifier) const;

		/**
		 * \brief add item attributes, call this before loadItems
		 */
		void addItemAttributes(const std::string & itemType, const std::vector<std::pair<std::string, std::string>> & itemAttributes) {
			_parser.addItemAttributes(itemType, itemAttributes);
		}

		/**
		 * \brief returns all parsed item types
		 */
		std::set<std::string> getItemTypes() const {
			return _parser.getItemTypes();
		}

	private:
		ItemParser _parser;

		ItemManager();
	};

} /* namespace item */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_ITEM_ITEMMANAGER_H__ */

/**
 * @}
 */
