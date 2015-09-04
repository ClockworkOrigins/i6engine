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

#include "i6engine/rpg/dialog/Dialog.h"

#include "i6engine/utils/i6eString.h"

namespace i6engine {
namespace rpg {
namespace dialog {

	Dialog::Dialog(const std::string & i, const std::string & p) : identifier(i), nr(0), participants(), permanent(false), important(false), description(), conditionScript(), infoScript(), conditionFunc(), infoFunc() {
		participants = utils::split(p, ";");
	}

	Dialog::~Dialog() {
	}

} /* namespace Dialog */
} /* namespace rpg */
} /* namespace i6engine */
