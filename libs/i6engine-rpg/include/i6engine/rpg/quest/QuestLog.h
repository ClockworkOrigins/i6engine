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

#ifndef __I6ENGINE_RPG_QUEST_QUESTLOG_H__
#define __I6ENGINE_RPG_QUEST_QUESTLOG_H__

#include <map>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

#include "i6engine/rpg/quest/QuestParser.h"

namespace i6e {
namespace rpg {
namespace quest {

	enum class QuestStatus;
	struct Quest;

	/**
	 * \brief represents a Questlog containing all quest informations and the state of the quest
	 */
	class ISIXE_RPG_API QuestLog : public utils::Singleton<QuestLog>, public api::MessageSubscriberFacade {
	public:
		QuestLog();

		void loadQuests(const std::string & directory) {
			_parser.loadQuests(directory);
			for (size_t i = 0; i < _parser._questTypes.size(); i++) {
				_quests.insert(std::make_pair(i, std::vector<Quest *>()));
			}
		}

		void show();

		void hide();

		bool isActive() const {
			return _active;
		}

		void setQuestStatus(const std::string & identifier, QuestStatus status);

		void addLogEntry(const std::string & identifier, const std::string & entry);

		std::string getQuestName(const std::string & identifier) const;

	private:
		std::map<size_t, std::vector<Quest *>> _quests;
		QuestParser _parser;
		bool _active;

		std::map<std::string, std::string> _questTypeMapping;
		std::map<std::string, Quest *> _questMapping;

		std::map<size_t, std::vector<Quest *>>::iterator getCategoryIterator(const std::string & category);

		void News(const api::GameMessage::Ptr & msg);
	};

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_QUEST_QUESTLOG_H__ */

/**
 * @}
 */
