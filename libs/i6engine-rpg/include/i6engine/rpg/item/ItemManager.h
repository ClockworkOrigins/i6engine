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

#ifndef __I6ENGINE_RPG_ITEM_ITEMMANAGER_H__
#define __I6ENGINE_RPG_ITEM_ITEMMANAGER_H__

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/rpg/item/ItemParser.h"

namespace i6engine {
namespace rpg {
namespace item {

	class ISIXE_RPG_API ItemManager : public utils::Singleton<ItemManager> {
		friend class utils::Singleton<ItemManager>;

	public:
		~ItemManager();

		void loadItems(const std::string & directory) {
			_parser.loadItems(directory);
		}

		void createItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot);

	private:
		ItemParser _parser;

		ItemManager();
	};

} /* namespace item */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_ITEM_ITEMMANAGER_H__ */
