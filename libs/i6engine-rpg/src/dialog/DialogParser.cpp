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

#include "i6engine/rpg/dialog/DialogParser.h"

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/rpg/dialog/Dialog.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6engine {
namespace rpg {
namespace dialog {

	DialogParser::DialogParser() : _dialogs() {
	}

	DialogParser::~DialogParser() {
		for (std::pair<std::string, Dialog *> p : _dialogs) {
			delete p.second;
		}
		_dialogs.clear();
	}

	void DialogParser::loadDialogs(const std::string & directory) {
		for (std::pair<std::string, Dialog *> p : _dialogs) {
			delete p.second;
		}
		_dialogs.clear();
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
						loadDialog(file);
					} else if (boost::filesystem::is_directory(*iter)) {
						std::string path = iter->path().string();
						directories.push(path);
					}
					iter++;
				}
			} catch (boost::filesystem::filesystem_error & e) {
				ISIXE_THROW_FAILURE("DialogParser", e.what());
			}
		}
	}

	void DialogParser::loadDialog(const std::string & file) {
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("DialogParser", "Couldn't open Dialog file (" << file << "): " << doc.GetErrorStr1());
			return;
		}

		for (tinyxml2::XMLElement * dialog = doc.FirstChildElement("Dialog"); dialog != nullptr; dialog = dialog->NextSiblingElement("Dialog")) {
			if (dialog->Attribute("identifier") == nullptr) {
				ISIXE_THROW_FAILURE("DialogParser", "Dialog in file '" << file << "' misses identifier!");
			}
			std::string identifier = dialog->Attribute("identifier");
			if (dialog->FirstChildElement("Participants") == nullptr) {
				ISIXE_THROW_FAILURE("DialogParser", "Dialog with identifier '" << identifier << "' in file '" << file << "' has no participants!");
			}
			std::string participants = dialog->FirstChildElement("Participants")->GetText();
			if (dialog->FirstChildElement("Info") == nullptr) {
				ISIXE_THROW_FAILURE("DialogParser", "Dialog with identifier '" << identifier << "' in file '" << file << "' has no info script function!");
			}
			std::string info = dialog->FirstChildElement("Info")->GetText();

			Dialog * d = new Dialog(identifier, participants);
			d->infoScript = info;

			if (dialog->FirstChildElement("Nr") != nullptr) {
				d->nr = std::stoul(dialog->FirstChildElement("Nr")->GetText());
			}
			if (dialog->FirstChildElement("Permanent") != nullptr) {
				std::string perm = dialog->FirstChildElement("Permanent")->GetText();
				std::transform(perm.begin(), perm.end(), perm.begin(), ::tolower);
				d->permanent = (perm == "true" || perm == "1");
			}
			if (dialog->FirstChildElement("Important") != nullptr) {
				std::string perm = dialog->FirstChildElement("Important")->GetText();
				std::transform(perm.begin(), perm.end(), perm.begin(), ::tolower);
				d->important = (perm == "true" || perm == "1");
			}
			if (dialog->FirstChildElement("Description") != nullptr) {
				d->description = dialog->FirstChildElement("Description")->GetText();
			}
			if (dialog->FirstChildElement("Condition") != nullptr) {
				d->conditionScript = dialog->FirstChildElement("Condition")->GetText();
			}

			_dialogs.insert(std::make_pair(identifier, d));
		}
	}

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */
