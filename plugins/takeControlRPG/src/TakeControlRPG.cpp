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

#include "TakeControlRPG.h"

#include <regex>

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/manager/TextManager.h"

#include "i6engine/rpg/dialog/DialogManager.h"
#include "i6engine/rpg/npc/NPCManager.h"
#include "i6engine/rpg/npc/NPCParser.h"

#include "boost/filesystem.hpp"

#include "clockUtils/iniParser/iniParser.h"

#include <QApplication>
#include <QMessageBox>
#include <QStringList>

namespace i6e {
namespace plugins {

	TakeControlRPG::TakeControlRPG() : _npcList(), _dialogList() {
		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setText(QApplication::tr("RPG.ini not found!"));
			box.exec();
			return;
		}
		std::string NPCDirectory;
		if (iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setText(QApplication::tr("No entry for npcDirectory in RPG.ini!"));
			box.exec();
			return;
		}
		rpg::npc::NPCParser::GetSingleton().loadNPCs(NPCDirectory);
		auto npcFileList = rpg::npc::NPCParser::GetSingleton().getNPCList();
		for (auto & p : npcFileList) {
			rpg::npc::NPCManager::GetSingleton().createNPC(p.first, Vec3::ZERO, Quaternion::IDENTITY, false);
			_npcList.push_back(rpg::npc::NPCManager::GetSingleton().getNPC(p.first));
		}

		std::string dialogDirectory;
		if (iniParser.getValue("SCRIPT", "dialogDirectory", dialogDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Error during startup!"));
			box.setInformativeText(QApplication::tr("'dialogDirectory' in section 'SCRIPT' in RPG.ini not found!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		rpg::dialog::DialogManager::GetSingletonPtr()->loadDialogs(dialogDirectory);

		std::map<std::string, rpg::dialog::Dialog *> dialogs = rpg::dialog::DialogManager::GetSingletonPtr()->getDialogs();
		for (auto & p : dialogs) {
			std::vector<std::tuple<std::string, std::string>> sentences;

#if ISIXE_SCRIPTING == SCRIPTING_LUA
			std::queue<std::string> directories;
			std::string scriptsPath;
			if (clockUtils::ClockError::SUCCESS != i6eEngineController->getIniParser().getValue("SCRIPT", "LuaScriptsPath", scriptsPath)) {
				return;
			}
			directories.push(scriptsPath);

			while (!directories.empty()) {
				std::string dir = directories.front();
				directories.pop();
				try {
					boost::filesystem::directory_iterator iter(dir), dirEnd;
					while (iter != dirEnd) {
						if (boost::filesystem::is_regular_file(*iter)) {
							std::string file = iter->path().string();
							std::ifstream in(file);
							bool foundFunction = false;
							while (in.good()) {
								std::string line;
								std::getline(in, line);
								if (!foundFunction) {
									if (std::regex_search(line, std::regex(p.second->infoScript))) {
										foundFunction = true;
									}
								} else {
									if (std::regex_search(line, std::regex("function "))) {
										break;
									} else {
										if (line.find(":say(") != std::string::npos) {
											std::string npc;
											std::string subtitleIdentifier;
											line = std::regex_replace(line, std::regex("[ \t]*"), "");
											std::vector<std::string> splitted = utils::split(line, ":");
											assert(splitted.size() == 2);
											if (splitted.size() != 2) {
												return;
											}
											npc = splitted[0];
											splitted = utils::split(line, "\"");
											assert(splitted.size() == 5);
											if (splitted.size() != 5) {
												return;
											}
											subtitleIdentifier = splitted[3];
											sentences.push_back(std::make_pair(npc, subtitleIdentifier));
										}
									}
		}
	}
						} else if (boost::filesystem::is_directory(*iter)) {
							std::string path = iter->path().string();
							directories.push(path);
						}
						iter++;
					}
				} catch (boost::filesystem::filesystem_error &) {
				}
			}
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
			std::queue<std::string> directories;
			std::string scriptsPath;
			if (clockUtils::ClockError::SUCCESS != i6eEngineController->getIniParser().getValue("SCRIPT", "PythonScriptsPath", scriptsPath)) {
				return;
			}
			directories.push(scriptsPath);

			while (!directories.empty()) {
				std::string dir = directories.front();
				directories.pop();
				try {
					boost::filesystem::directory_iterator iter(dir), dirEnd;
					while (iter != dirEnd) {
						if (boost::filesystem::is_regular_file(*iter)) {
							std::string file = iter->path().string();
							std::ifstream in(file);
							bool foundFunction = false;
							while (in.good()) {
								std::string line;
								std::getline(in, line);
								if (!foundFunction) {
									if (std::regex_search(line, std::regex(p.second->infoScript))) {
										foundFunction = true;
									}
								} else {
									if (std::regex_search(line, std::regex("def "))) {
										break;
									} else {
										if (line.find(".say(") != std::string::npos) {
											std::string npc;
											std::string subtitleIdentifier;
											line = std::regex_replace(line, std::regex("[ \t]*"), "");
											std::vector<std::string> splitted = utils::split(line, ".");
											assert(splitted.size() == 2);
											if (splitted.size() != 2) {
												return;
											}
											npc = splitted[0];
											splitted = utils::split(line, "\"");
											assert(splitted.size() == 5);
											if (splitted.size() != 5) {
												return;
											}
											subtitleIdentifier = splitted[3];
											sentences.push_back(std::make_pair(npc, subtitleIdentifier));
										}
									}
								}
							}
						} else if (boost::filesystem::is_directory(*iter)) {
							std::string path = iter->path().string();
							directories.push(path);
						}
						iter++;
					}
				} catch (boost::filesystem::filesystem_error &) {
				}
			}
#endif

			_dialogList.push_back(std::make_tuple(p.second, sentences));
		}
	}

	std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>>> TakeControlRPG::getDialogs() {
		return _dialogList;
	}

	std::vector<rpg::npc::NPC *> TakeControlRPG::getNPCs() {
		return _npcList;
	}

	QString TakeControlRPG::getSubtitleText(QString identifier) const {
		return QString::fromLatin1(i6eEngineController->getTextManager()->getText(identifier.toStdString()).c_str());
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.takeControl.script.DialogPluginInterface")
