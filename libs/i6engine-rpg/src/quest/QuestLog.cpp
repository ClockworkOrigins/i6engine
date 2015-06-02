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

#include "i6engine/rpg/quest/QuestLog.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/manager/TextManager.h"

#include "i6engine/rpg/quest/Quest.h"

namespace i6engine {
namespace rpg {
namespace quest {

	QuestLog::QuestLog() : _quests(), _parser(), _active(false), _questTypeMapping(), _questMapping() {
	}

	void QuestLog::show() {
		_active = true;

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		gf->addStatusList("QuestTypeList", "RPG/Listbox", 0.05, 0.1, -1);
		gf->setSize("QuestTypeList", 0.2, 0.8);
		gf->setFont("QuestTypeList", "DejaVuSans-10");

		for (std::string s : _parser._questTypes) {
			gf->addTextToWidget("QuestTypeList", api::EngineController::GetSingleton().getTextManager()->getText(s));
			_questTypeMapping.insert(std::make_pair(api::EngineController::GetSingleton().getTextManager()->getText(s), s));
		}

		gf->setSelectedStringCallback("QuestTypeList", [this, gf](std::string s) {
			assert(_questTypeMapping.find(s) != _questTypeMapping.end());
			std::string category = _questTypeMapping[s];
			gf->clearWidget("QuestList");

			for (size_t i = 0; i < _parser._questTypes.size(); i++) {
				if (_parser._questTypes[i] == category) {
					auto it = _quests.find(i);
					if (it != _quests.end()) {
						for (Quest * q : it->second) {
							gf->addTextToWidget("QuestList", api::EngineController::GetSingleton().getTextManager()->getText(q->name));
							_questMapping.insert(std::make_pair(api::EngineController::GetSingleton().getTextManager()->getText(q->name), q));
						}
					}
					break;
				}
			}
		});

		gf->addStatusList("QuestList", "RPG/Listbox", 0.3, 0.1, -1);
		gf->setSize("QuestList", 0.2, 0.8);
	}

	void QuestLog::hide() {
		_active = false;

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		gf->deleteWidget("QuestTypeList");
		gf->deleteWidget("QuestList");

		_questMapping.clear();
	}

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6engine */
