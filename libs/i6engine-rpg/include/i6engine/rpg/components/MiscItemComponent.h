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

#ifndef __I6ENGINE_RPG_COMPONENTS_MISCITEMCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_MISCITEMCOMPONENT_H__

#include "i6engine/rpg/components/ItemComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	/**
	 * \brief a misc item is an item that that can do nothing like gold
	 */
	class ISIXE_RPG_API MiscItemComponent : public ItemComponent {
	public:
		MiscItemComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "MiscItem";
		}

		bool use(const api::GOPtr & self) override {
			return false;
		}

		std::vector<std::pair<std::string, std::string>> getInfos() const override;

	private:
		void Init() override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_MISCITEMCOMPONENT_H__ */

/**
 * @}
 */
