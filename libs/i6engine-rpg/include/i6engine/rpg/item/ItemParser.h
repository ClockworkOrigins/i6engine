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

#ifndef __I6ENGINE_RPG_ITEM_ITEMPARSER_H__
#define __I6ENGINE_RPG_ITEM_ITEMPARSER_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/api/configs/ObjectConfig.h"

namespace i6engine {
namespace rpg {
namespace item {

	class ItemManager;

	class ISIXE_RPG_API ItemParser {
		friend class ItemManager;

	public:
		ItemParser();
		~ItemParser();

		void loadItems(const std::string & directory);

		std::pair<std::string, i6engine::api::objects::GOTemplate> getTemplate(const std::string & identifier) const;

	private:
		std::map<std::string, std::pair<std::string, i6engine::api::objects::GOTemplate>> _items;

		void loadItem(const std::string & file);
	};

} /* namespace item */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_ITEM_ITEMPARSER_H__ */
