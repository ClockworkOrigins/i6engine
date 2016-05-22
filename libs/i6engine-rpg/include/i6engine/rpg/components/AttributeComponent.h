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

#ifndef __I6ENGINE_RPG_COMPONENTS_ATTRIBUTECOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_ATTRIBUTECOMPONENT_H__

#include "i6engine/utils/AutoUpdater.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace rpg {
namespace components {

	enum class Attribute {
		HP,
		HP_MAX,
		COUNT
	};

	/**
	 * \brief container for attributes and their values
	 */
	class ISIXE_RPG_API AttributeComponent : public api::Component {
	public:
		AttributeComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const override {
			return "Attribute";
		}

		/**
		 * \brief returns the value of given attribute
		 */
		int32_t getAttributeValue(Attribute attribute) const {
			return _attributes[size_t(attribute)];
		}

		/**
		 * \brief changes the value of the attribute by given diff value
		 */
		void changeAttribute(Attribute attribute, int32_t diff);

		/**
		 * \brief registers a callback being notified on change of the attribute
		 */
		void registerListener(Attribute attribute, const std::function<void(int32_t)> & func);

	private:
		std::vector<utils::AutoUpdater<int32_t>> _attributes;

		void Init() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_ATTRIBUTECOMPONENT_H__ */

/**
 * @}
 */
