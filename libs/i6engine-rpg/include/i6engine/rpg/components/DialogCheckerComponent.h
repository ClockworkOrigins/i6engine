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

#ifndef __I6ENGINE_RPG_COMPONENTS_DIALOGCHECKERCOMPONENT_H__
#define __I6ENGINE_RPG_COMPONENTS_DIALOGCHECKERCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {
	class PhysicalStateComponent;
} /* namespace api */
namespace rpg {
namespace components {

	/**
	 * \brief Component for characters with dialogs
	 * This component checks every tick when in talk distance to player if it has an important dialog to start
	 */
	class ISIXE_RPG_API DialogCheckerComponent : public api::Component {
	public:
		DialogCheckerComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		api::attributeMap synchronize() const override;

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const override {
			return "DialogChecker";
		}

		std::string getNPCIdentifier() const {
			return _identifier;
		}

	private:
		utils::weakPtr<api::PhysicalStateComponent, api::Component> _psc;
		api::WeakGOPtr _player;
		std::string _identifier;

		void Init() override;

		void Finalize() override;

		void Tick() override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}
	};

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_COMPONENTS_DIALOGCHECKERCOMPONENT_H__ */

/**
 * @}
 */
