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

#ifndef __SAMPLE_SCRIPTING_COMPONENTS_SCRIPTINGSHATTERCOMPONENT_H__
#define __SAMPLE_SCRIPTING_COMPONENTS_SCRIPTINGSHATTERCOMPONENT_H__

#include "i6engine/api/components/ShatterComponent.h"

namespace sample {
namespace components {

	class ScriptingShatterComponent : public i6engine::api::ShatterComponent {
	public:
		ScriptingShatterComponent(int64_t id, const i6engine::api::attributeMap & params);

		static i6engine::api::ComPtr createC(int64_t id, const i6engine::api::attributeMap & params);

		void Init() override;

		void Finalize() override;

		i6engine::api::attributeMap synchronize() const override;

		std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return std::make_pair(i6engine::api::AddStrategy::REJECT, -1);
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const {
			return "ScriptingShatter";
		}

		void Tick() override;

		void shatter(const i6engine::api::GOPtr & other);
	};

} /* namespace components */
} /* namespace sample */

#endif /* __SAMPLE_SCRIPTING_COMPONENTS_SCRIPTINGSHATTERCOMPONENT_H__ */

/**
 * @}
 */
