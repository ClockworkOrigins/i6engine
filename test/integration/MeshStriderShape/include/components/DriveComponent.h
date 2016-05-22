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
 * \addtogroup integration
 * @{
 */

#ifndef __I6ENGINE_INTEGRATION_COMPONENTS_DRIVECOMPONENT_H__
#define __I6ENGINE_INTEGRATION_COMPONENTS_DRIVECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_INTEGRATION_COMPONENTS_DRIVECOMPONENT_H__ */

/**
 * @}
 */
