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

#ifndef __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace rpg {
namespace components {

	class ISIXE_RPG_API ItemComponent : public api::Component {
	public:
		ItemComponent(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr & comp) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief self is using this item, return value decides whether item is removed from inventory or not
		 */
		virtual bool use(const api::GOPtr & self) = 0;

		std::string getImageset() const {
			return _imageset;
		}

		std::string getImage() const {
			return _image;
		}

	private:
		uint32_t _value;
		std::string _imageset;
		std::string _image;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__ */

/**
 * @}
 */
