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

#ifndef __I6ENGINE_RPG_CONFIG_EXTERNALCONSTANTS_H__
#define __I6ENGINE_RPG_CONFIG_EXTERNALCONSTANTS_H__

#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace rpg {
namespace config {

	/**
	 * \brief distance for NPCs starting to check if they have an important dialog
	 */
	extern ISIXE_RPG_API double NPC_CHECK_TALK_DISTANCE;

	/**
	 * \brief distance for NPCs starting to talk if they have an important dialog or they can be talked to
	 */
	extern ISIXE_RPG_API double NPC_TALK_DISTANCE;

	/**
	 * \brief time per word a subtitle is shown
	 */
	extern ISIXE_RPG_API double SUBTITLE_TIME_PER_WORD;

	/**
	 * \brief maximum distance where dialog sounds should be hearable
	 */
	extern ISIXE_RPG_API double DIALOG_MAX_DISTANCE;

	/**
	 * \brief item identifier for used currency
	 */
	extern ISIXE_RPG_API std::string CURRENCY;

	/**
	 * \brief specifies whether traders should have limited gold or not
	 */
	extern ISIXE_RPG_API bool LIMITED_TRADER_GOLD;

	/**
	 * \brief parses external constants in specified file
	 */
	ISIXE_RPG_API void parseExternalConstants(const std::string & file);

} /* namespace config */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_CONFIG_EXTERNALCONSTANTS_H__ */

/**
 * @}
 */
