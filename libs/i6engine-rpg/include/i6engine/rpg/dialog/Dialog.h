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

#ifndef __I6ENGINE_RPG_DIALOG_DIALOG_H__
#define __I6ENGINE_RPG_DIALOG_DIALOG_H__

#include <cstdint>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6engine {
namespace rpg {
namespace dialog {

	struct ISIXE_RPG_API Dialog {
		Dialog(const std::string & i, const std::string & p);
		~Dialog();

		std::string identifier;
		uint32_t nr;
		std::vector<std::string> participants;
		bool permanent;
		bool important;
		std::string description;
		std::string conditionScript;
		std::string infoScript;
	};

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_DIALOG_DIALOG_H__ */
