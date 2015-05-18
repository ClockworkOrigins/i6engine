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

#include "i6engine/rpg/dialog/DialogManager.h"

#include "i6engine/rpg/dialog/Dialog.h"

namespace i6engine {
namespace rpg {
namespace dialog {

	DialogManager::DialogManager() : _parser(), _npcDialogs() {
	}

	DialogManager::~DialogManager() {
	}

	void DialogManager::loadDialogs(const std::string & directory) {
		_parser.loadDialogs(directory);

		for (auto & p : _parser._dialogs) {
			for (std::string & s : p.second->participants) {
				_npcDialogs[s].push_back(p.second);
			}
		}

		for (auto it = _npcDialogs.begin(); it != _npcDialogs.end(); it++) {
			std::sort(it->second.begin(), it->second.end(), [](Dialog * a, Dialog * b) {
				return a->nr < b->nr;
			});
		}
	}

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */
