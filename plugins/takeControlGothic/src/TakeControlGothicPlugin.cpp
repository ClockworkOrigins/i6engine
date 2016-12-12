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

#include "TakeControlGothicPlugin.h"

#include <iostream>
#include <set>
#include <stack>
#include <thread>

#include "TakeControlGothicWidget.h"

#include "i6engine/api/configs/ObjectConfig.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/npc/NPC.h"

#include <QDirIterator>
#include <QFuture>
#include <QProgressDialog>
#include <QtConcurrent>
#include <QTextStream>

namespace i6e {
namespace plugins {

	TakeControlGothicPlugin::TakeControlGothicPlugin() : _npcList(), _dialogList(), _subtitleToTextMap(), _lastPath() {
	}

	void TakeControlGothicPlugin::gothicPathChanged(QString path) {
		if (path == _lastPath) {
			return;
		}
		for (rpg::npc::NPC * npc : _npcList) {
			delete npc;
		}
		_npcList.clear();
		for (auto & t : _dialogList) {
			delete std::get<0>(t);
		}
		_dialogList.clear();
		_subtitleToTextMap.clear();

		QDirIterator it(path, QStringList() << "*.d", QDir::Files, QDirIterator::Subdirectories);

		enum class SearchResult {
			Nothing,
			NPC,
			Dialog,
			Function
		};

		QStringList files;
		while (it.hasNext()) {
			QString fileName = it.filePath();
			files.append(fileName);
			it.next();
		}

		QProgressDialog * dlg = new QProgressDialog("Parsing scripts", "", 0, files.size());
		dlg->setCancelButton(nullptr);
		connect(this, SIGNAL(updatedData()), dlg, SLOT(deleteLater()));
		connect(this, SIGNAL(updateParseProgress(int)), dlg, SLOT(setValue(int)));

		std::thread([this, files]() {
			int parsed = 0;
			// Function Name => Dialog
			std::map<QString, rpg::dialog::Dialog *> infoToDialogMap;
			// Function Name => List of (Talker, SubtitleText)
			std::map<QString, std::vector<std::tuple<QString, QString>>> otherFunctionsMap;
			// Choice => Parent
			std::multimap<QString, QString> choicesMap;
			QMutex * mutex = new QMutex();
			QFuture<void> future = QtConcurrent::map(files, [this, mutex, &infoToDialogMap, &otherFunctionsMap, &choicesMap, &parsed](QString filePath) {
				SearchResult result = SearchResult::Nothing;
				int bracketCount = 0;
				QFile f(filePath);
				if (f.open(QIODevice::ReadOnly)) {
					QTextStream reader(&f);
					QString npcIdentifier;
					QString dialogInfo;
					QString currentFunction;
					rpg::dialog::Dialog * dlg = nullptr;
					while (!reader.atEnd()) {
						QString line = reader.readLine();
						if (result == SearchResult::Nothing) {
							if (line.contains("INSTANCE ", Qt::CaseSensitivity::CaseInsensitive)) {
								if (line.contains("(Npc_Default)", Qt::CaseSensitivity::CaseInsensitive) || line.contains("(C_NPC)", Qt::CaseSensitivity::CaseInsensitive) || line.contains("(Mst_", Qt::CaseSensitivity::CaseInsensitive)) {
									result = SearchResult::NPC;
									QString identifier = line.replace("INSTANCE", "", Qt::CaseSensitivity::CaseInsensitive);
									identifier = identifier.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
									identifier = identifier.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
									QStringList l = identifier.split("(Npc_default)", QString::SplitBehavior::KeepEmptyParts, Qt::CaseSensitivity::CaseInsensitive);
									if (l.size() > 0) {
										identifier = l.front();
									}
									l = identifier.split("(C_NPC)", QString::SplitBehavior::KeepEmptyParts, Qt::CaseSensitivity::CaseInsensitive);
									if (l.size() > 0) {
										identifier = l.front();
									}
									l = identifier.split("(Mst_", QString::SplitBehavior::KeepEmptyParts, Qt::CaseSensitivity::CaseInsensitive);
									if (l.size() > 0) {
										identifier = l.front();
									}
									rpg::npc::NPC * npc = new rpg::npc::NPC(identifier.toStdString(), api::objects::GOTemplate(), false);
									QMutexLocker ml(mutex);
									_npcList.push_back(npc);
								} else if (line.contains("(C_INFO)", Qt::CaseSensitivity::CaseInsensitive)) {
									result = SearchResult::Dialog;
									QString identifier = line.replace("INSTANCE", "", Qt::CaseSensitivity::CaseInsensitive);
									identifier = identifier.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
									identifier = identifier.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
									QStringList l = identifier.split("(C_INFO)", QString::SplitBehavior::KeepEmptyParts, Qt::CaseSensitivity::CaseInsensitive);
									if (l.size() > 0) {
										identifier = l.front();
									}
									dlg = new rpg::dialog::Dialog(identifier.toStdString(), "PC_Hero");
								}
							} else if (line.contains("FUNC ", Qt::CaseSensitivity::CaseInsensitive)) {
								QString identifier = line.replace("FUNC", "", Qt::CaseSensitivity::CaseInsensitive);
								identifier = identifier.replace("(\\s*)", " ");
								identifier = identifier.replace("(\\t*)", " ");
								QStringList l = identifier.split(" ", QString::SplitBehavior::SkipEmptyParts);
								assert(l.size() > 1);
								if (l.size() <= 1) {
									continue;
								}
								identifier = l[1];
								l = identifier.split("(");
								assert(l.size() >= 1);
								identifier = l[0];
								currentFunction = identifier;
								result = SearchResult::Function;
							}
							bracketCount += line.count("{");
							bracketCount -= line.count("}");
							if (bracketCount == 0 && line.count("{") > 0) {
								result = SearchResult::Nothing;
							}
						} else if (result == SearchResult::NPC) {
							bracketCount += line.count("{");
							bracketCount -= line.count("}");
							if (bracketCount == 0) {
								result = SearchResult::Nothing;
							}
						} else if (result == SearchResult::Dialog) {
							if (line.contains(QRegExp("[ \t]*npc[ \t]*="))) {
								npcIdentifier = line.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
								npcIdentifier = npcIdentifier.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
								QStringList l = npcIdentifier.split("=");
								assert(l.size() == 2);
								npcIdentifier = l[1];
								l = npcIdentifier.split(";");
								assert(l.size() == 1);
								npcIdentifier = l[0];
								assert(dlg);
								dlg->participants.push_back(npcIdentifier.toStdString());
							} else if (line.contains(QRegExp("[ \t]*information[ \t]*="))) {
								dialogInfo = line.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
								dialogInfo = dialogInfo.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
								QStringList l = dialogInfo.split("=");
								assert(l.size() == 2);
								dialogInfo = l[1];
								l = dialogInfo.split(";");
								assert(l.size() == 1);
								dialogInfo = l[0];
								assert(dlg);
								dlg->infoScript = dialogInfo.toStdString();
							}
							bracketCount += line.count("{");
							bracketCount -= line.count("}");
							if (bracketCount == 0) {
								{
									QMutexLocker ml(mutex);
									_dialogList.push_back(std::make_tuple(dlg, std::vector<std::tuple<std::string, std::string>>()));
									infoToDialogMap.insert(std::make_pair(QString::fromStdString(dlg->infoScript), dlg));
								}
								dlg = nullptr;
								result = SearchResult::Nothing;
							}
						} else if (result == SearchResult::Function) {
							if (line.contains("AI_Output")) {
								QStringList l = line.split("//");
								assert(l.size() == 2);
								if (l.size() != 2) {
									continue;
								}
								QString text = l[1];
								line = l[0];
								line = line.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace("AI_Output", "", Qt::CaseSensitivity::CaseSensitive);
								line = line.replace("(", "", Qt::CaseSensitivity::CaseSensitive);
								line = line.replace(");", "", Qt::CaseSensitivity::CaseSensitive);
								line = line.replace("\"", "", Qt::CaseSensitivity::CaseSensitive);
								l = line.split(",");
								assert(l.size() == 3);
								if (l.size() != 3) {
									continue;
								}
								QString talker = l[0];
								if (talker.compare("hero", Qt::CaseSensitivity::CaseInsensitive) == 0 || talker.compare("other", Qt::CaseSensitivity::CaseInsensitive) == 0) {
									talker = "PC_Hero";
								} else if (talker.compare("self", Qt::CaseSensitivity::CaseInsensitive) == 0) {
									QMutexLocker ml(mutex);
									auto it = infoToDialogMap.find(currentFunction);
									if (it != infoToDialogMap.end()) {
										assert(it->second->participants.size() >= 2);
										if (it->second->participants.size() < 2) {
											continue;
										}
										talker = QString::fromStdString(it->second->participants[1]); // second one is always NPC, first one always Player
									}
								}
								QString subtitleIdentifier = l[2];
								QMutexLocker ml(mutex);
								_subtitleToTextMap.insert(std::make_pair(subtitleIdentifier, text));
								auto it = infoToDialogMap.find(currentFunction);
								if (it != infoToDialogMap.end()) {
									for (size_t i = 0; i < _dialogList.size(); i++) {
										if (std::get<0>(_dialogList[i]) == it->second) {
											std::get<1>(_dialogList[i]).push_back(std::make_pair(talker.toStdString(), subtitleIdentifier.toStdString()));
											break;
										}
									}
								} else {
									otherFunctionsMap[currentFunction].push_back(std::make_tuple(talker, subtitleIdentifier));
								}
							} else if (line.contains("Info_AddChoice", Qt::CaseSensitivity::CaseInsensitive)) {
								line = line.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace("Info_AddChoice", "", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace("(", "", Qt::CaseSensitivity::CaseSensitive);
								line = line.replace(");", "", Qt::CaseSensitivity::CaseSensitive);
								line = line.replace(QRegExp("\".*\""), " ");
								QStringList l = line.split(",");
								assert(l.size() == 3);
								if (l.size() != 3) {
									continue;
								}
								QMutexLocker ml(mutex);
								choicesMap.insert(std::make_pair(l[2], currentFunction));
							} else if (line.contains(QRegExp("[a-zA-Z0-9_]*[ \t]*\\(.*\\);"))) {
								line = line.split("(").front();
								while (line.endsWith(" ") || line.endsWith("\t")) {
									line.resize(line.size() - 1);
								}
								line = line.replace("\t", " ", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace(QRegExp("\\s\\s+"), " ");
								line = line.split(QRegExp("\\s")).back();
								line = line.replace(" ", "", Qt::CaseSensitivity::CaseInsensitive);
								line = line.replace("\t", "", Qt::CaseSensitivity::CaseInsensitive);
								QMutexLocker ml(mutex);
								choicesMap.insert(std::make_pair(line, currentFunction));
							}
							bracketCount += line.count("{");
							bracketCount -= line.count("}");
							if (bracketCount == 0) {
								result = SearchResult::Nothing;
							}
						}
					}
				}
				parsed++;
				emit updateParseProgress(parsed);
			});
			future.waitForFinished();
			delete mutex;
			for (int i = 0; i < 2; i++) {
				for (auto it = otherFunctionsMap.begin(); it != otherFunctionsMap.end();) {
					auto choiceIt = choicesMap.find(it->first);
					if (choiceIt != choicesMap.end()) {
						// it is a choice
						auto dialogIt = infoToDialogMap.find(choiceIt->second);
						std::set<QString> visited;
						QString func = choiceIt->first;
						visited.insert(func);
						std::stack<QString> parentStack;
						parentStack.push(func);
						while (dialogIt == infoToDialogMap.end()) {
							while (visited.find(choiceIt->second) != visited.end()) {
								choiceIt++;
								if (choiceIt == choicesMap.end()) {
									break;
								}
							}
							QString last;
							if (choiceIt != choicesMap.end()) {
								last = choiceIt->second;
								choiceIt = choicesMap.find(choiceIt->second);
							}
							if (choiceIt == choicesMap.end()) {
								if (!last.isEmpty()) {
									dialogIt = infoToDialogMap.find(last);
									if (dialogIt != infoToDialogMap.end()) {
										break;
									}
								}
								parentStack.pop();
								if (parentStack.empty()) {
									break;
								}
								choiceIt = choicesMap.find(parentStack.top());
								continue;
							}
							func = choiceIt->first;
							visited.insert(func);
							parentStack.push(func);
							dialogIt = infoToDialogMap.find(choiceIt->second);
						}
						assert(dialogIt != infoToDialogMap.end());
						if (dialogIt == infoToDialogMap.end()) {
							it++;
							continue;
						}
						// Create new dialog for Choice
						rpg::dialog::Dialog * dlg = new rpg::dialog::Dialog(it->first.toStdString(), "");
						dlg->participants = dialogIt->second->participants;
						_dialogList.push_back(std::make_tuple(dlg, std::vector<std::tuple<std::string, std::string>>()));
						for (auto & t : it->second) {
							QString talker = std::get<0>(t);
							if (dlg->participants.size() >= 2) {
								talker = talker.replace("self", QString::fromStdString(dlg->participants[1]), Qt::CaseSensitivity::CaseInsensitive);
							}
							QString subtitleIdentifier = std::get<1>(t);
							std::get<1>(_dialogList.back()).push_back(std::make_tuple(talker.toStdString(), subtitleIdentifier.toStdString()));
						}
						it = otherFunctionsMap.erase(it);
					} else {
						it++;
					}
				}
			}
			std::cout << "Found " << _dialogList.size() << " Dialogs" << std::endl;
			std::cout << "Found " << otherFunctionsMap.size() << " Functions with dialogs" << std::endl;
			emit updatedData();
		}).detach();
	}

	std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>>> TakeControlGothicPlugin::getDialogs() {
		return _dialogList;
	}

	std::vector<rpg::npc::NPC *> TakeControlGothicPlugin::getNPCs() {
		return _npcList;
	}

	QString TakeControlGothicPlugin::getSubtitleText(QString identifier) const {
		return _subtitleToTextMap.at(identifier);
	}

	takeControl::widgets::SettingsWidget * TakeControlGothicPlugin::getSettingsWidget(clockUtils::iniParser::IniParser * iniParser, QWidget * par) {
		TakeControlGothicWidget * w = new TakeControlGothicWidget(iniParser, par);
		connect(w, SIGNAL(gothicPathChanged(QString)), this, SLOT(gothicPathChanged(QString)));
		gothicPathChanged(w->getGothicPath());
		return w;
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.takeControl.script.DialogPluginInterface")
