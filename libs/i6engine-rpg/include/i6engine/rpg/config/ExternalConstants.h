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
