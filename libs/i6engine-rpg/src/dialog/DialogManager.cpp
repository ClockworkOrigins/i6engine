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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ScriptingFacade.h"

#include "i6engine/rpg/dialog/Dialog.h"

namespace i6engine {
namespace rpg {
namespace dialog {

	DialogManager::DialogManager() : _parser(), _npcDialogs(), _importantChecks(), _showDialogboxChecks(), _dialogActive(false), _lock(), _heardDialogs() {
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

	void DialogManager::checkImportantDialogs(const std::string & identifier) {
		// while in a dialog, important dialogs mustn't be checked
		if (_dialogActive) {
			return;
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
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(identifier);
		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
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

	bool DialogManager::wasHeard(const std::string & identifier) const {
		auto it = _parser._dialogs.find(identifier);
		if (it != _parser._dialogs.end()) {
			std::lock_guard<std::mutex> lg(_lock);
			return _heardDialogs.find(it->second) != _heardDialogs.end();
		}
		return false;
	}

	bool DialogManager::checkDialogsLoop() {
		// while a dialog is active new important dialogs mustn't start
		if (_dialogActive) {
			_importantChecks.clear();
		}
		while (!_importantChecks.empty()) {
			auto t = _importantChecks.poll();
			if (std::get<DialogCheck::Result>(t)->get()) {
				// if an important dialog was found, drop all other results (if dialog is really executed) and start dialog
				if (runDialog(std::get<DialogCheck::NPCIdentifier>(t), std::get<DialogCheck::DialogIdentifier>(t))) {
					_importantChecks.clear();
					_showDialogboxChecks.clear();
				}
			}
		}
		while (!_showDialogboxChecks.empty()) {
			auto t = _showDialogboxChecks.poll();
			if (std::get<DialogCheck::Result>(t)->get()) {
				// if an important dialog was found, drop all other results (if dialog is really executed) and start dialog
				showDialog(std::get<DialogCheck::NPCIdentifier>(t), std::get<DialogCheck::DialogIdentifier>(t));
			}
		}

		return true;
	}

	bool DialogManager::runDialog(const std::string & npc, const std::string & dia) {
		// TODO: (Daniel) check whether NPC is still in dialog range

		// get dialog
		std::lock_guard<std::mutex> lg(_lock);
		auto it = _npcDialogs.find(npc);

		if (it != _npcDialogs.end()) {
			for (Dialog * d : it->second) {
				if (d->identifier == dia) {
					api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(d->infoScript);
					_heardDialogs.insert(d);
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

	}

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */
