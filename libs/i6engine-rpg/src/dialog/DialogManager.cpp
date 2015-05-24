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
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/HealthbarComponent.h"
#include "i6engine/rpg/components/QuickslotComponent.h"
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

	DialogManager::DialogManager() : api::MessageSubscriberFacade(), _parser(), _npcDialogs(), _importantChecks(), _showDialogboxChecks(), _dialogActive(false), _lock(), _heardDialogs(), _running(true), _worker(std::bind(&DialogManager::checkDialogsLoop, this)), _guiInitialized(false), _activeNPCs(), _dialogMapping(), _dialogNumberVector(), _showDialogCalls(0), _subtitles(true), _dialogNumbers(false) {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, DialogManager::News, this);
	}

	DialogManager::~DialogManager() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
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
		std::cout << "Checking Important" << std::endl;
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(identifier);
		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
				if (d->important) {
					std::cout << "Checking: " << d->identifier << std::endl;
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
				auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
				if (playerList.empty()) {
					return;
				}
				auto player = *playerList.begin();
				auto qc = player->getGOC<components::QuickslotComponent>(components::config::ComponentTypes::QuickslotComponent);
				if (qc != nullptr) {
					qc->show();
				}
				auto hc = player->getGOC<components::HealthbarComponent>(components::config::ComponentTypes::HealthbarComponent);
				if (hc != nullptr) {
					hc->show();
				}
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
			processMessages();
			// while a dialog is active new important dialogs mustn't start
			if (_dialogActive) {
				_importantChecks.clear();
			}
			while (!_importantChecks.empty()) {
				auto t = _importantChecks.poll();
				std::cout << "Checking Important Loop" << std::endl;
				if (std::get<DialogCheck::Result>(t)->get()) {
					std::cout << "Checking Important Loop 2" << std::endl;
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
					std::cout << "Checking Important Loop 3" << std::endl;

					// if an important dialog was found, drop all other results (if dialog is really executed) and start dialog
					if (allNear && runDialog(std::get<DialogCheck::NPCIdentifier>(t), std::get<DialogCheck::DialogIdentifier>(t))) {
						std::cout << "Checking Important Loop 4" << std::endl;
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
					_dialogNumberVector.clear();
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
					// hide gui elements
					auto qc = player->getGOC<components::QuickslotComponent>(components::config::ComponentTypes::QuickslotComponent);
					if (qc != nullptr) {
						qc->hide();
					}
					auto hc = player->getGOC<components::HealthbarComponent>(components::config::ComponentTypes::HealthbarComponent);
					if (hc != nullptr) {
						hc->hide();
					}
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
		std::string description = api::EngineController::GetSingleton().getTextManager()->getText(it->second->description);

		if (_dialogNumbers) {
			description = std::to_string(_dialogMapping.size() + 1) + ": " + description;
		}

		_dialogNumberVector.push_back(dia);
		_dialogMapping.insert(std::make_pair(description, dia));

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		gf->addTextToWidget("DialogList", description);
	}

	void DialogManager::News(const api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == api::messages::InputMessageType) {
			if (msg->getSubtype() == api::keyboard::KeyboardMessageTypes::KeyKeyboard) {
				api::input::Input_Keyboard_Update * iku = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent());

				if (iku->pressed == api::KeyState::KEY_PRESSED) {
					if (iku->code == api::KeyCode::KC_1 && _dialogNumberVector.size() >= 1) {
						runDialog(_activeNPCs[0], _dialogNumberVector[0]);
					} else if (iku->code == api::KeyCode::KC_2 && _dialogNumberVector.size() >= 2) {
						runDialog(_activeNPCs[0], _dialogNumberVector[1]);
					} else if (iku->code == api::KeyCode::KC_3 && _dialogNumberVector.size() >= 3) {
						runDialog(_activeNPCs[0], _dialogNumberVector[2]);
					} else if (iku->code == api::KeyCode::KC_4 && _dialogNumberVector.size() >= 4) {
						runDialog(_activeNPCs[0], _dialogNumberVector[3]);
					} else if (iku->code == api::KeyCode::KC_5 && _dialogNumberVector.size() >= 5) {
						runDialog(_activeNPCs[0], _dialogNumberVector[4]);
					} else if (iku->code == api::KeyCode::KC_6 && _dialogNumberVector.size() >= 6) {
						runDialog(_activeNPCs[0], _dialogNumberVector[5]);
					} else if (iku->code == api::KeyCode::KC_7 && _dialogNumberVector.size() >= 7) {
						runDialog(_activeNPCs[0], _dialogNumberVector[6]);
					} else if (iku->code == api::KeyCode::KC_8 && _dialogNumberVector.size() >= 8) {
						runDialog(_activeNPCs[0], _dialogNumberVector[7]);
					} else if (iku->code == api::KeyCode::KC_9 && _dialogNumberVector.size() >= 9) {
						runDialog(_activeNPCs[0], _dialogNumberVector[8]);
					} else if (iku->code == api::KeyCode::KC_0 && _dialogNumberVector.size() >= 10) {
						runDialog(_activeNPCs[0], _dialogNumberVector[9]);
					}
				}
			}
		}
	}

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */
