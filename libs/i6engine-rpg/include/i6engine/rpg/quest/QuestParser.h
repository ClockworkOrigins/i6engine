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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_QUEST_QUESTPARSER_H__
#define __I6ENGINE_RPG_QUEST_QUESTPARSER_H__

#include <map>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace rpg {
namespace quest {

	struct Quest;
	class QuestLog;

	/**
	 * \brief parses and stores Quests
	 */
	class ISIXE_RPG_API QuestParser {
		friend class QuestLog;

	public:
		QuestParser();
		~QuestParser();

		void loadQuests(const std::string & directory);

	private:
		std::vector<std::string> _questTypes;
		std::map<std::string, Quest *> _quests;

		void loadQuest(const std::string & file);
	};

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_QUEST_QUESTPARSER_H__ */

/**
 * @}
 */
