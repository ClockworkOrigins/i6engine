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
 * \addtogroup integration
 * @{
 */

#ifndef __I6ENGINE_INTEGRATION_COMPONENTS_DRIVECOMPONENT_H__
#define __I6ENGINE_INTEGRATION_COMPONENTS_DRIVECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace integration {
namespace components {

	class DriveComponent : public api::Component {
	public:
		DriveComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		void Init() override;

		void Finalize() override;

		api::attributeMap synchronize() const override;

		std::pair<api::AddStrategy, int64_t> howToAdd(const api::ComPtr &) const override {
			return std::make_pair(api::AddStrategy::REJECT, -1);
		}

		std::vector<api::componentOptions> getComponentOptions() override {
			return {};
		}

		std::string getTemplateName() const {
			return "Drive";
		}

		void Tick() override;

	private:
		uint32_t _counter;
		double _sum;
		uint64_t _sumCounter;
	};

} /* namespace components */
} /* namespace integration */
} /* namespace i6engine */

#endif /* __I6ENGINE_INTEGRATION_COMPONENTS_DRIVECOMPONENT_H__ */

/**
 * @}
 */
