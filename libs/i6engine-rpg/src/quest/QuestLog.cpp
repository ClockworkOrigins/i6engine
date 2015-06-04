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
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/manager/TextManager.h"

#include "i6engine/rpg/quest/Quest.h"

namespace i6engine {
namespace rpg {
namespace quest {

	QuestLog::QuestLog() : _quests(), _parser(), _active(false), _questTypeMapping(), _questMapping() {
	}

	void QuestLog::show() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, QuestLog::News, this);
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
			gf->clearWidget("QuestEntriesList");

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

		gf->setSelectedStringCallback("QuestList", [this, gf](std::string s) {
			assert(_questMapping.find(s) != _questMapping.end());
			Quest * q = _questMapping[s];
			gf->clearWidget("QuestEntriesList");

			for (size_t i = 0; i < q->entries.size(); i++) {
				gf->addTextToWidget("QuestEntriesList", api::EngineController::GetSingleton().getTextManager()->getText(q->entries[i]));
				if (i < q->entries.size() - 1) {
					gf->addTextToWidget("QuestEntriesList", " "); // add an empty line between two entries
				}
			}
		});

		gf->addStatusList("QuestEntriesList", "RPG/Listbox", 0.5, 0.1, -1);
		gf->setSize("QuestEntriesList", 0.45, 0.8);
		gf->setAutoLineBreak("QuestEntriesList", true);

		api::EngineController::GetSingleton().registerTimer(100000, [this]() {
			processMessages();
			return _active;
		}, true, 1);
	}

	void QuestLog::hide() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		_active = false;

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();

		gf->deleteWidget("QuestTypeList");
		gf->deleteWidget("QuestList");
		gf->deleteWidget("QuestEntriesList");

		_questMapping.clear();
	}

	void QuestLog::setQuestStatus(const std::string & identifier, QuestStatus status) {
		auto it = _parser._quests.find(identifier);
		if (it == _parser._quests.end()) {
			ISIXE_THROW_FAILURE("QuestLog", "Quest with identifier '" << identifier << "' not found!");
		}
		if (status == QuestStatus::LOCKED) {
			// reset quest?
			if (it->second->status == QuestStatus::RUNNING) {
				auto it2 = getCategoryIterator(it->second->category);
				for (size_t i = 0; i < it2->second.size(); i++) {
					if (it2->second[i] == it->second) {
						it2->second.erase(it2->second.begin() + int(i));
						break;
					}
				}
			} else if (it->second->status == QuestStatus::COMPLETED) {
				auto it2 = getCategoryIterator("CompletedQuest");
				for (size_t i = 0; i < it2->second.size(); i++) {
					if (it2->second[i] == it->second) {
						it2->second.erase(it2->second.begin() + int(i));
						break;
					}
				}
			} else if (it->second->status == QuestStatus::FAILED) {
				auto it2 = getCategoryIterator("FailedQuest");
				for (size_t i = 0; i < it2->second.size(); i++) {
					if (it2->second[i] == it->second) {
						it2->second.erase(it2->second.begin() + int(i));
						break;
					}
				}
			}
			it->second->status = QuestStatus::LOCKED;
		} else if (status == QuestStatus::RUNNING) {
			// start quest
			if (it->second->status == QuestStatus::LOCKED) {
				auto it2 = getCategoryIterator(it->second->category);
				it2->second.push_back(it->second);
				it->second->status = QuestStatus::RUNNING;
			}
		} else if (status == QuestStatus::COMPLETED) {
			// complete quest
			if (it->second->status == QuestStatus::RUNNING) {
				auto it2 = getCategoryIterator(it->second->category);
				for (size_t i = 0; i < it2->second.size(); i++) {
					if (it2->second[i] == it->second) {
						it2->second.erase(it2->second.begin() + int(i));
						break;
					}
				}
			}
			auto it2 = getCategoryIterator("CompletedQuest");
			it2->second.push_back(it->second);
			it->second->status = QuestStatus::COMPLETED;
		} else if (status == QuestStatus::FAILED) {
			// failed quest
			if (it->second->status == QuestStatus::RUNNING) {
				auto it2 = getCategoryIterator(it->second->category);
				for (size_t i = 0; i < it2->second.size(); i++) {
					if (it2->second[i] == it->second) {
						it2->second.erase(it2->second.begin() + int(i));
						break;
					}
				}
			}
			auto it2 = getCategoryIterator("FailedQuest");
			it2->second.push_back(it->second);
			it->second->status = QuestStatus::FAILED;
		}
	}

	void QuestLog::addLogEntry(const std::string & identifier, const std::string & entry) {
		auto it = _parser._quests.find(identifier);
		if (it == _parser._quests.end()) {
			ISIXE_THROW_FAILURE("QuestLog", "Quest with identifier '" << identifier << "' not found!");
		}
		it->second->entries.push_back(entry);
	}

	std::map<size_t, std::vector<Quest *>>::iterator QuestLog::getCategoryIterator(const std::string & category) {
		for (size_t i = 0; i < _parser._questTypes.size(); i++) {
			if (_parser._questTypes[i] == category) {
				return _quests.find(i);
			}
		}
		return _quests.end();
	}

	std::string QuestLog::getQuestName(const std::string & identifier) const {
		auto it = _parser._quests.find(identifier);
		if (it == _parser._quests.end()) {
			ISIXE_THROW_FAILURE("QuestLog", "Quest with identifier '" << identifier << "' not found!");
		}
		return api::EngineController::GetSingleton().getTextManager()->getText(it->second->name);
	}

	void QuestLog::News(const api::GameMessage::Ptr & msg) {
		if (!_active) {
			return;
		}
		if (msg->getMessageType() == api::messages::InputMessageType) {
			if (msg->getSubtype() == api::keyboard::KeyboardMessageTypes::KeyKeyboard) {
				api::KeyCode kc = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->code;
				api::KeyState ks = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->pressed;
				if (kc == api::KeyCode::KC_ESCAPE && ks == api::KeyState::KEY_PRESSED) {
					hide();
				}
			}
		}
	}

} /* namespace quest */
} /* namespace rpg */
} /* namespace i6engine */
