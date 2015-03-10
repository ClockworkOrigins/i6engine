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

#ifndef __I6ENGINE_RPG_COMPONENTS_USABLEITEMCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_USABLEITEMCOMPONENT_H__

#include "i6engine/rpg/components/ItemComponent.h"

namespace i6engine {
namespace rpg {

	enum class Attribute;

namespace components {

	/**
	 * \brief a usable item is an item that changes attributes of the user and is removed from inventory afterwards
	 */
	class UsableItemComponent : public ItemComponent {
	public:
		UsableItemComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		void Init() override;

		api::attributeMap synchronize() override;

		std::string getTemplateName() const {
			return "UsableItem";
		}

		bool use(const api::GOPtr & self);

	private:
		std::map<Attribute, int32_t> _attributeChange;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_USABLEITEMCOMPONENT_H__ */

/**
 * @}
 */
