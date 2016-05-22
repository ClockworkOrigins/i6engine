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

#ifndef __I6ENGINE_RPG_COMPONENTS_MISCITEMCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_MISCITEMCOMPONENT_H__

#include "i6engine/rpg/components/ItemComponent.h"

namespace i6e {
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

		bool use(const api::GOPtr &) override {
			return false;
		}

		std::vector<std::pair<std::string, std::string>> getInfos() const override;

	private:
		void Init() override;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_MISCITEMCOMPONENT_H__ */

/**
 * @}
 */
