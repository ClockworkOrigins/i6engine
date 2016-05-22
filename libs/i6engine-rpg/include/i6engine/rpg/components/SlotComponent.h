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

#ifndef __I6ENGINE_RPG_COMPONENTS_SLOTCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_SLOTCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_SLOTCOMPONENT_H__ */

/**
 * @}
 */
