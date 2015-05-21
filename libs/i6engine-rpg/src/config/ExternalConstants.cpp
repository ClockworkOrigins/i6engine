/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
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

#include "i6engine/rpg/config/ExternalConstants.h"

#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ptree.hpp"

namespace i6engine {
namespace rpg {
namespace config {

	double NPC_CHECK_TALK_DISTANCE = 10.0;
	double NPC_TALK_DISTANCE = 7.5;
	double SUBTITLE_TIME_PER_WORD = 0.5;
	double DIALOG_MAX_DISTANCE = 15.0;

	void parseExternalConstants(const std::string & file) {
		boost::property_tree::ptree pt;
		boost::property_tree::read_info(file, pt);

		NPC_CHECK_TALK_DISTANCE = pt.get("NPC_CHECK_TALK_DISTANCE", NPC_CHECK_TALK_DISTANCE);
		NPC_TALK_DISTANCE = pt.get("NPC_TALK_DISTANCE", NPC_TALK_DISTANCE);
		SUBTITLE_TIME_PER_WORD = pt.get("SUBTITLE_TIME_PER_WORD", SUBTITLE_TIME_PER_WORD);
		DIALOG_MAX_DISTANCE = pt.get("DIALOG_MAX_DISTANCE", DIALOG_MAX_DISTANCE);
	}

} /* namespace config */
} /* namespace rpg */
} /* namespace i6engine */
