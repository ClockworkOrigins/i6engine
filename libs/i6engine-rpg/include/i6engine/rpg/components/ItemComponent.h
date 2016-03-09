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

#ifndef __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace rpg {
namespace components {

	/**
	 * \brief base class for all items
	 * derive from it and implement virtual methods to create own behavior
	 */
	class ISIXE_RPG_API ItemComponent : public api::Component {
	public:
		ItemComponent(int64_t id, const api::attributeMap & params);

		virtual api::attributeMap synchronize() const override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		/**
		 * \brief self is using this item, return value decides whether item is removed from inventory or not
		 */
		virtual bool use(const api::GOPtr & self) = 0;

		uint32_t getValue() const {
			return _value;
		}

		std::string getImageset() const {
			return _imageset;
		}

		std::string getImage() const {
			return _image;
		}

		double getWeight() const {
			return _weight;
		}

		std::string getItemIdentifier() const {
			return _identifier;
		}

		/**
		 * \brief only used by SlotInventory to determine whether items of same type only take one slot or more
		 */
		bool isStackable() const {
			return _stackable;
		}

		/**
		 * \brief returns key value pairs for info screen for this item in inventory
		 */
		virtual std::vector<std::pair<std::string, std::string>> getInfos() const = 0;

	private:
		uint32_t _value;
		std::string _imageset;
		std::string _image;
		double _weight;
		std::string _identifier;
		bool _stackable;
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_COMPONENTS_ITEMCOMPONENT_H__ */

/**
 * @}
 */
