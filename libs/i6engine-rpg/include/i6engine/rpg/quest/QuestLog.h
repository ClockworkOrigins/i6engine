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

#ifndef __I6ENGINE_RPG_QUEST_QUESTLOG_H__
#define __I6ENGINE_RPG_QUEST_QUESTLOG_H__

#include <map>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/rpg/quest/QuestParser.h"

namespace i6engine {
namespace rpg {
namespace quest {

	struct Quest;

	class ISIXE_RPG_API QuestLog : public utils::Singleton<QuestLog> {
	public:
		QuestLog();

		void loadQuests(const std::string & directory) {
			_parser.loadQuests(directory);
		}

		void show();

		void hide();

		bool isActive() const {
			return _active;
		}

	private:
		std::map<size_t, std::vector<Quest *>> _quests;
		QuestParser _parser;
		bool _active;

		std::map<std::string, std::string> _questTypeMapping;
		std::map<std::string, Quest *> _questMapping;
	};

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_QUEST_QUESTLOG_H__ */
