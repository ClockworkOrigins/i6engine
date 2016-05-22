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

#include "i6engine/rpg/quest/QuestParser.h"

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/rpg/quest/Quest.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6e {
namespace rpg {
namespace quest {

	QuestParser::QuestParser() : _questTypes(), _quests() {
	}

	QuestParser::~QuestParser() {
		for (std::pair<std::string, Quest *> p : _quests) {
			delete p.second;
		}
		_quests.clear();
	}

	void QuestParser::loadQuests(const std::string & directory) {
		std::queue<std::string> directories;
		directories.push(directory);

		while (!directories.empty()) {
			std::string dir = directories.front();
			directories.pop();
			try {
				boost::filesystem::directory_iterator iter(dir), dirEnd;
				while (iter != dirEnd) {
					if (boost::filesystem::is_regular_file(*iter)) {
						std::string file = iter->path().string();
						loadQuest(file);
					} else if (boost::filesystem::is_directory(*iter)) {
						std::string path = iter->path().string();
						directories.push(path);
					}
					iter++;
				}
			} catch (boost::filesystem::filesystem_error & e) {
				ISIXE_THROW_FAILURE("QuestParser", e.what());
			}
		}

		for (auto & p : _quests) {
			auto it = std::find(_questTypes.begin(), _questTypes.end(), p.second->category);
			if (it == _questTypes.end()) {
				ISIXE_THROW_FAILURE("QuestParser", "Quest category '" << p.second->category << "' used for quest '" << p.first << "' not registered!");
			}
		}
	}

	void QuestParser::loadQuest(const std::string & file) {
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("QuestParser", "Couldn't open Quest file (" << file << "): " << doc.GetErrorStr1());
			return;
		}

		for (tinyxml2::XMLElement * category = doc.FirstChildElement("Category"); category != nullptr; category = category->NextSiblingElement("Category")) {
			if (category->Attribute("identifier") == nullptr) {
				ISIXE_THROW_FAILURE("QuestParser", "Quest categoriy in file '" << file << "' misses identifier!");
			}
			std::string identifier = category->Attribute("identifier");

			_questTypes.push_back(identifier);
		}
		for (tinyxml2::XMLElement * quest = doc.FirstChildElement("Quest"); quest != nullptr; quest = quest->NextSiblingElement("Quest")) {
			if (quest->Attribute("identifier") == nullptr) {
				ISIXE_THROW_FAILURE("QuestParser", "Quest in file '" << file << "' misses identifier!");
			}
			std::string identifier = quest->Attribute("identifier");
			if (quest->FirstChildElement("Name") == nullptr) {
				ISIXE_THROW_FAILURE("QuestParser", "Quest with identifier '" << identifier << "' in file '" << file << "' has no name!");
			}
			std::string name = quest->FirstChildElement("Name")->GetText();
			if (quest->FirstChildElement("Category") == nullptr) {
				ISIXE_THROW_FAILURE("QuestParser", "Quest with identifier '" << identifier << "' in file '" << file << "' has no category!");
			}
			std::string category = quest->FirstChildElement("Category")->GetText();

			Quest * q = new Quest();
			q->name = name;
			q->category = category;

			if (quest->FirstChildElement("Level") != nullptr) {
				q->level = std::stoul(quest->FirstChildElement("Level")->GetText());
			}

			_quests.insert(std::make_pair(identifier, q));
		}
	}

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6e */
