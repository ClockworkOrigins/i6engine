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

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
#include "i6engine/rpg/config/ExternalConstants.h"
#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/gui/Config.h"
#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/NPCManager.h"
#include "i6engine/rpg/npc/queueJobs/ExitDialogJob.h"
#include "i6engine/rpg/npc/queueJobs/ShowDialogsJob.h"

namespace i6engine {
namespace rpg {
namespace dialog {

	DialogManager::DialogManager() : _parser(), _npcDialogs(), _importantChecks(), _showDialogboxChecks(), _dialogActive(false), _lock(), _heardDialogs(), _running(true), _worker(std::bind(&DialogManager::checkDialogsLoop, this)), _guiInitialized(false), _activeNPCs(), _dialogMapping(), _showDialogCalls(0) {
	}

	DialogManager::~DialogManager() {
		_running = false;
		_worker.join();
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

	void DialogManager::checkImportantDialogs(const std::string & identifier) {
		// while in a dialog, important dialogs mustn't be checked
		if (_dialogActive) {
			return;
		}
		if (!_guiInitialized) {
			_guiInitialized = true;
			api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
			gf->addImage("DialogBox", "RPG/StaticImage", "RPG", "TbM_Filling", 0.2, 0.75, 0.6, 0.2);
			gf->addStatusList("DialogList", "RPG/Listbox", 0.21, 0.76, -1);
			gf->setSize("DialogList", 0.58, 0.18);
			gf->setVisibility("DialogBox", false);
			gf->setVisibility("DialogList", false);
			gf->createWidget("SubtitleWidget", "Subtitle", "RPG/StaticImage");
			gf->setProperty("SubtitleWidget", "Image", "RPG/TbM_Filling");
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, gui::SetListboxStyle, core::Method::Update, new gui::GUI_SetListboxStyle("SubtitleWidget", "RPG/Listbox"), core::Subsystem::Unknown));
			gf->setPosition("SubtitleWidget", 0.2, 0.05);
			gf->setSize("SubtitleWidget", 0.6, 0.25);
			gf->setVisibility("SubtitleWidget", false);
		}
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(identifier);
		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
				if (d->important) {
					if (d->conditionScript.empty()) {
						auto r = std::make_shared<utils::Future<bool>>();
						r->push(true);
						_importantChecks.push(std::make_tuple(identifier, d->identifier, r));
					} else {
						_importantChecks.push(std::make_tuple(identifier, d->identifier, api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(d->conditionScript)));
					}
				}
			}
		}
	}

	void DialogManager::checkDialogs(const std::string & identifier) {
		if (_showDialogCalls >= 0) {
			_showDialogCalls++;
		}
		if (_showDialogCalls < int8_t(_activeNPCs.size() + 1)) {
			if (_showDialogCalls == -int8_t(_activeNPCs.size()) - 1) {
				_dialogActive = false;
				_showDialogCalls = 0;
				_activeNPCs.clear();
			}
			return;
		}
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		if (!_guiInitialized) {
			_guiInitialized = true;
			gf->addImage("DialogBox", "RPG/StaticImage", "RPG", "TbM_Filling", 0.2, 0.75, 0.6, 0.2);
			gf->addStatusList("DialogList", "RPG/Listbox", 0.21, 0.76, -1);
			gf->setSize("DialogList", 0.58, 0.18);
			gf->createWidget("SubtitleWidget", "Subtitle", "RPG/StaticImage");
			gf->setProperty("SubtitleWidget", "Image", "RPG/TbM_Filling");
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, gui::SetListboxStyle, core::Method::Update, new gui::GUI_SetListboxStyle("SubtitleWidget", "RPG/Listbox"), core::Subsystem::Unknown));
			gf->setPosition("SubtitleWidget", 0.2, 0.05);
			gf->setSize("SubtitleWidget", 0.6, 0.25);
			gf->setVisibility("SubtitleWidget", false);
		} else {
			gf->setVisibility("DialogBox", true);
			gf->setVisibility("DialogList", true);
		}
		gf->setSelectedStringCallback("DialogList", [this, identifier](std::string s) {
			if (!_dialogMapping.empty()) {
				runDialog(identifier, _dialogMapping[s]);
			}
		});
		_dialogActive = true;
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(identifier);
		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
				if (!d->important) {
					if (d->conditionScript.empty()) {
						auto r = std::make_shared<utils::Future<bool>>();
						r->push(true);
						_showDialogboxChecks.push(std::make_tuple(identifier, d->identifier, r));
					} else {
						_showDialogboxChecks.push(std::make_tuple(identifier, d->identifier, api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(d->conditionScript)));
					}
				}
			}
		}
	}

	bool DialogManager::wasHeard(const std::string & identifier) const {
		auto it = _parser._dialogs.find(identifier);
		if (it != _parser._dialogs.end()) {
			std::lock_guard<std::mutex> lg(_lock);
			return _heardDialogs.find(it->second) != _heardDialogs.end();
		}
		return false;
	}

	void DialogManager::exitDialog() {
		auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
		auto player = *playerList.begin();
		npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
		p->addJob(new npc::ExitDialogJob());
		for (std::string s : _activeNPCs) {
			npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(s);
			n->addJob(new npc::ExitDialogJob());
		}
	}

	bool DialogManager::checkDialogsLoop() {
		while (_running) {
			// while a dialog is active new important dialogs mustn't start
			if (_dialogActive) {
				_importantChecks.clear();
			}
			while (!_importantChecks.empty()) {
				auto t = _importantChecks.poll();
				if (std::get<DialogCheck::Result>(t)->get()) {
					// dialog can be run, but we have to check the distance between the participants
					auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
					if (playerList.empty()) {
						break;
					}
					auto player = *playerList.begin();

					auto it = _parser._dialogs.find(std::get<DialogCheck::DialogIdentifier>(t));
					if (it == _parser._dialogs.end()) {
						break;
					}
					npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
					if (p == nullptr) {
						break;
					}
					bool allNear = true;
					for (std::string npcIdentifier : it->second->participants) {
						npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(npcIdentifier);
						if (n == nullptr) {
							allNear = false;
							break;
						}
						auto go = n->getGO();
						if (go == nullptr) {
							allNear = false;
							break;
						}
						if ((go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition() - player->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition()).length() <= config::NPC_TALK_DISTANCE) {
							n->turnToNPC(p);
						} else {
							if ((go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition() - player->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition()).length() <= config::NPC_CHECK_TALK_DISTANCE) {
								n->turnToNPC(p);
							}
							allNear = false;
							break;
						}
					}

					// if an important dialog was found, drop all other results (if dialog is really executed) and start dialog
					if (allNear && runDialog(std::get<DialogCheck::NPCIdentifier>(t), std::get<DialogCheck::DialogIdentifier>(t))) {
						_importantChecks.clear();
						_showDialogboxChecks.clear();
					}
				}
			}
			while (!_showDialogboxChecks.empty()) {
				auto t = _showDialogboxChecks.poll();
				if (std::get<DialogCheck::Result>(t)->get()) {
					// dialog can be run, but we have to check the distance between the participants
					auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
					if (playerList.empty()) {
						break;
					}
					auto player = *playerList.begin();

					auto it = _parser._dialogs.find(std::get<DialogCheck::DialogIdentifier>(t));
					if (it == _parser._dialogs.end()) {
						break;
					}
					npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
					if (p == nullptr) {
						break;
					}
					bool allNear = true;
					for (std::string npcIdentifier : it->second->participants) {
						npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(npcIdentifier);
						if (n == nullptr) {
							allNear = false;
							break;
						}
						auto go = n->getGO();
						if (go == nullptr) {
							allNear = false;
							break;
						}
						if ((go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition() - player->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition()).length() <= config::NPC_TALK_DISTANCE) {
							n->turnToNPC(p);
						} else {
							allNear = false;
							break;
						}
					}

					// if an important dialog was found, drop all other results (if dialog is really executed) and start dialog
					if (allNear) {
						showDialog(std::get<DialogCheck::NPCIdentifier>(t), std::get<DialogCheck::DialogIdentifier>(t));
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		return true;
	}

	bool DialogManager::runDialog(const std::string & npc, const std::string & dia) {
		// get dialog
		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		gf->clearWidget("DialogList");
		gf->setVisibility("DialogBox", false);
		gf->setVisibility("DialogList", false);
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(npc);
		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
				if (d->identifier == dia) {
					_dialogMapping.clear();
					_showDialogCalls = 0;
					_heardDialogs.insert(d);
					_dialogActive = true;
					auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
					auto player = *playerList.begin();
					npc::NPC * nt = npc::NPCManager::GetSingleton().getNPC(*d->participants.begin());
					npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
					p->turnToNPC(nt);
					api::EngineController::GetSingleton().getScriptingFacade()->callFunctionWithCallback<void>(d->infoScript, [d]() {
						auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
						auto player = *playerList.begin();
						npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
						p->addJob(new npc::ShowDialogsJob(*d->participants.begin()));
						for (std::string s : d->participants) {
							npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(s);
							n->addJob(new npc::ShowDialogsJob(*d->participants.begin()));
						}
					});
					_activeNPCs = d->participants;
					if (!d->permanent) {
						for (auto p : d->participants) {
							auto it2 = _npcDialogs.find(p);
							if (it2 != _npcDialogs.end()) {
								for (size_t i = 0; i < it2->second.size(); i++) {
									if (it2->second[i] == d) {
										it2->second.erase(it2->second.begin() + int(i));
										break;
									}
								}
							}
						}
					}
					return true;
				}
			}
		}

		return false;
	}

	void DialogManager::showDialog(const std::string & npc, const std::string & dia) {
		auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
		auto player = *playerList.begin();

		auto it = _parser._dialogs.find(dia);
		npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
		npc::NPC * nt = npc::NPCManager::GetSingleton().getNPC(*it->second->participants.begin());
		p->turnToNPC(nt);
		for (std::string npcIdentifier : it->second->participants) {
			npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(npcIdentifier);
			if (n == nullptr) {
				break;
			}
			auto go = n->getGO();
			if (go == nullptr) {
				break;
			}
			n->turnToNPC(p);
		}
		_dialogMapping.insert(std::make_pair(api::EngineController::GetSingleton().getTextManager()->getText(it->second->description), dia));

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		gf->addTextToWidget("DialogList", api::EngineController::GetSingleton().getTextManager()->getText(it->second->description));
	}

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */
