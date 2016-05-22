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

#include "i6engine/rpg/config/ExternalConstants.h"

#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ptree.hpp"

#include <iostream>

namespace i6e {
namespace rpg {
namespace config {

	double NPC_CHECK_TALK_DISTANCE = 10.0;
	double NPC_TALK_DISTANCE = 7.5;
	double SUBTITLE_TIME_PER_WORD = 0.5;
	double DIALOG_MAX_DISTANCE = 15.0;
	std::string CURRENCY = "Gold";
	bool LIMITED_TRADER_GOLD = false;

	void parseExternalConstants(const std::string & file) {
		boost::property_tree::ptree pt;
		boost::property_tree::read_info(file, pt);

		NPC_CHECK_TALK_DISTANCE = pt.get("NPC_CHECK_TALK_DISTANCE", NPC_CHECK_TALK_DISTANCE);
		NPC_TALK_DISTANCE = pt.get("NPC_TALK_DISTANCE", NPC_TALK_DISTANCE);
		SUBTITLE_TIME_PER_WORD = pt.get("SUBTITLE_TIME_PER_WORD", SUBTITLE_TIME_PER_WORD);
		DIALOG_MAX_DISTANCE = pt.get("DIALOG_MAX_DISTANCE", DIALOG_MAX_DISTANCE);
		CURRENCY = pt.get("CURRENCY", CURRENCY);
		LIMITED_TRADER_GOLD = pt.get("LIMITED_TRADER_GOLD", LIMITED_TRADER_GOLD);
	}

} /* namespace config */
} /* namespace rpg */
} /* namespace i6e */
