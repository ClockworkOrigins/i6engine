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

namespace i6e {
namespace rpg {
namespace dialog {

	DialogManager::DialogManager() : api::MessageSubscriberFacade(), _parser(), _npcDialogs(), _importantChecks(), _showDialogboxChecks(), _dialogActive(false), _lock(), _heardDialogs(), _running(true), _jobID(), _guiInitialized(false), _activeNPCs(), _dialogMapping(), _dialogNumberVector(), _showDialogCalls(0), _subtitles(true), _subtitlePosition(0.2, 0.05), _subtitleSize(0.6, 0.25), _subtitleFont("DejaVuSans-8"), _dialogNumbers(false) {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, DialogManager::News, this);
		_jobID = api::EngineController::GetSingleton().registerTimer(10000, boost::bind(&DialogManager::checkDialogsLoop, this), true, core::JobPriorities::Prio_Medium);
	}

	DialogManager::~DialogManager() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		_running = false;
		api::EngineController::GetSingleton().removeTimerID(_jobID);
	}

	void DialogManager::loadDialogs(const std::string & directory) {
		_npcDialogs.clear();
		_parser.loadDialogs(directory);

		for (auto & p : _parser._dialogs) {
			for (std::string & s : p.second->participants) {
				_npcDialogs[s].push_back(p.second);
			}
		}

		for (auto it = _npcDialogs.begin(); it != _npcDialogs.end(); it++) {
			std::sort(it->second.begin(), it->second.end(), [](Dialog * a, Dialog * b) {
				return a->nr <= b->nr;
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
			gf->setPosition("SubtitleWidget", _subtitlePosition.getX(), _subtitlePosition.getY());
			gf->setSize("SubtitleWidget", _subtitleSize.getX(), _subtitleSize.getY());
			gf->setFont("SubtitleWidget", _subtitleFont);
			gf->setVisibility("SubtitleWidget", false);
		}
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(identifier);
		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
				if (d->important) {
					if (d->conditionScript.empty()) {
						if (d->conditionFunc == nullptr) {
							auto r = std::make_shared<utils::Future<bool>>();
							r->push(true);
							_importantChecks.push(std::make_tuple(identifier, d->identifier, r));
						} else {
							auto r = std::make_shared<utils::Future<bool>>();
							r->push(d->conditionFunc());
							_importantChecks.push(std::make_tuple(identifier, d->identifier, r));
						}
					} else {
#if ISIXE_SCRIPTING != SCRIPTING_NONE
						_importantChecks.push(std::make_tuple(identifier, d->identifier, api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(d->conditionScript)));
#else
						ISIXE_THROW_FAILURE("DialogManager", "Dialog '" << identifier << "' has condition script but engine was built without scripting!");
#endif
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
			gf->setPosition("SubtitleWidget", _subtitlePosition.getX(), _subtitlePosition.getY());
			gf->setSize("SubtitleWidget", _subtitleSize.getX(), _subtitleSize.getY());
			gf->setFont("SubtitleWidget", _subtitleFont);
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
						if (d->conditionFunc == nullptr) {
							auto r = std::make_shared<utils::Future<bool>>();
							r->push(true);
							_showDialogboxChecks.push(std::make_tuple(identifier, d->identifier, r));
						} else {
							auto r = std::make_shared<utils::Future<bool>>();
							r->push(d->conditionFunc());
							_showDialogboxChecks.push(std::make_tuple(identifier, d->identifier, r));
						}
					} else {
#if ISIXE_SCRIPTING != SCRIPTING_NONE
						_showDialogboxChecks.push(std::make_tuple(identifier, d->identifier, api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(d->conditionScript)));
#else
						ISIXE_THROW_FAILURE("DialogManager", "Dialog '" << identifier << "' has condition script but engine was built without scripting!");
#endif
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

	void DialogManager::setSubtitlePosition(const Vec2 & pos) {
		_subtitlePosition = pos;
		if (_guiInitialized) {
			api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
			gf->setPosition("SubtitleWidget", _subtitlePosition.getX(), _subtitlePosition.getY());
		}
	}

	void DialogManager::setSubtitleSize(const Vec2 & size) {
		_subtitleSize = size;
		if (_guiInitialized) {
			api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
			gf->setSize("SubtitleWidget", _subtitleSize.getX(), _subtitleSize.getY());
		}
	}

	void DialogManager::setSubtitleFont(const std::string & font) {
		_subtitleFont = font;
		if (_guiInitialized) {
			api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
			gf->setFont("SubtitleWidget", _subtitleFont);
		}
	}

	void DialogManager::reset() {
		_guiInitialized = false;
	}

	bool DialogManager::checkDialogsLoop() {
		processMessages();
		// while a dialog is active new important dialogs mustn't start
		if (_dialogActive) {
			_importantChecks.clear();
		}
		while (!_importantChecks.empty()) {
			std::tuple<std::string, std::string, std::shared_ptr<utils::Future<bool>>> t;
			_importantChecks.poll(t); // Daniel: no error check necessary as it is polled just from one thread
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
			std::tuple<std::string, std::string, std::shared_ptr<utils::Future<bool>>> t;
			_showDialogboxChecks.poll(t); // Daniel: no error check necessary as it is polled just from one thread
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
		return _running;
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
					if (d->infoScript.empty() && d->infoFunc == nullptr) {
						ISIXE_THROW_FAILURE("DialogManager", "Dialog '" << d->identifier << "' has no info script or function!");
#if ISIXE_SCRIPTING != SCRIPTING_NONE
					} else if (!d->infoScript.empty()) {
						api::EngineController::GetSingleton().getScriptingFacade()->callFunctionWithCallback<void>(d->infoScript, [d]() {
							auto playerList2 = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
							auto player2 = *playerList2.begin();
							npc::NPC * p2 = npc::NPCManager::GetSingleton().getNPC(player2->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());
							p2->addJob(new npc::ShowDialogsJob(*d->participants.begin()));
							for (std::string s : d->participants) {
								npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(s);
								n->addJob(new npc::ShowDialogsJob(*d->participants.begin()));
							}
						});
#endif
					} else if (d->infoFunc == nullptr) {
						ISIXE_THROW_FAILURE("DialogManager", "Dialog '" << d->identifier << "' has no info function!");
					} else {
						d->infoFunc();
						p->addJob(new npc::ShowDialogsJob(*d->participants.begin()));
						for (std::string s : d->participants) {
							npc::NPC * n = npc::NPCManager::GetSingleton().getNPC(s);
							n->addJob(new npc::ShowDialogsJob(*d->participants.begin()));
						}
					}
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
						for (auto p2 : d->participants) {
							auto it2 = _npcDialogs.find(p2);
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
		_activeNPCs.push_back(npc);
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
			description = std::to_string(_dialogMapping.size() + 1) + ". " + description;
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
} /* namespace i6e */
