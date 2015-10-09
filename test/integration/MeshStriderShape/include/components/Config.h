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

#ifndef __I6ENGINE_INTEGRATION_COMPONENTS_CONFIG_H__
#define __I6ENGINE_INTEGRATION_COMPONENTS_CONFIG_H__

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
namespace integration {
namespace components {
namespace config {

	enum ComponentTypes {
		DriveComponent = i6engine::api::components::ComponentTypes::ComponentTypesCount
	};

} /* namespace config */
} /* namespace components */
} /* namespace integration */
} /* namespace i6engine */

#endif /* __I6ENGINE_INTEGRATION_COMPONENTS_CONFIG_H__ */

/**
 * @}
 */