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

#ifndef __I6ENGINE_RPG_COMPONENTS_SLOTCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_SLOTCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace rpg {
namespace components {

	/**
	 * \brief add this Component to your items if you use SlotInventory
	 * specifies the amount of slots being used by this item
	 */
	class ISIXE_RPG_API SlotComponent : public api::Component {
	public:
		SlotComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const override {
			return "Slot";
		}

		uint16_t getWidth() const {
			return _width;
		}

		uint16_t getHeight() const {
			return _height;
		}

	private:
		uint16_t _width;
		uint16_t _height;

		void Init() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_SLOTCOMPONENT_H__ */

/**
 * @}
 */
