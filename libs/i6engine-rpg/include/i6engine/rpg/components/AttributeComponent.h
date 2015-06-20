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

#ifndef __I6ENGINE_RPG_COMPONENTS_ATTRIBUTECOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_ATTRIBUTECOMPONENT_H__

#include "i6engine/utils/AutoUpdater.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace rpg {
namespace components {

	enum class Attribute {
		HP,
		HP_MAX,
		COUNT
	};

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

		int32_t getAttributeValue(Attribute attribute) const {
			return _attributes[size_t(attribute)];
		}

		void changeAttribute(Attribute attribute, int32_t diff);

		void registerListener(Attribute attribute, const std::function<void(int32_t)> & func);

	private:
		std::vector<utils::AutoUpdater<int32_t>> _attributes;

		void Init() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr & comp) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_ATTRIBUTECOMPONENT_H__ */

/**
 * @}
 */
