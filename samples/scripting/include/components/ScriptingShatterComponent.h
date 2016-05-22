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
 * \addtogroup rpg
 * @{
 */

#ifndef __SAMPLE_SCRIPTING_COMPONENTS_SCRIPTINGSHATTERCOMPONENT_H__
#define __SAMPLE_SCRIPTING_COMPONENTS_SCRIPTINGSHATTERCOMPONENT_H__

#include "i6engine/api/components/ShatterComponent.h"

namespace sample {
namespace components {

	class ScriptingShatterComponent : public i6e::api::ShatterComponent {
	public:
		ScriptingShatterComponent(int64_t id, const i6e::api::attributeMap & params);

		static i6e::api::ComPtr createC(int64_t id, const i6e::api::attributeMap & params);

		void Init() override;

		void Finalize() override;

		i6e::api::attributeMap synchronize() const override;

		std::pair<i6e::api::AddStrategy, int64_t> howToAdd(const i6e::api::ComPtr &) const override {
			return std::make_pair(i6e::api::AddStrategy::REJECT, -1);
		}

		std::vector<i6e::api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const {
			return "ScriptingShatter";
		}

		void Tick() override;

		void shatter(const i6e::api::GOPtr & other);
	};

} /* namespace components */
} /* namespace sample */

#endif /* __SAMPLE_SCRIPTING_COMPONENTS_SCRIPTINGSHATTERCOMPONENT_H__ */

/**
 * @}
 */
