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

#ifndef __I6ENGINE_RPG_CONFIG_EXTERNALCONSTANTS_H__
#define __I6ENGINE_RPG_CONFIG_EXTERNALCONSTANTS_H__

#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6engine {
namespace rpg {
namespace config {

	extern ISIXE_RPG_API double NPC_CHECK_TALK_DISTANCE;

	ISIXE_RPG_API void parseExternalConstants(const std::string & file);

} /* namespace config */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_CONFIG_EXTERNALCONSTANTS_H__ */

/**
 * @}
 */
