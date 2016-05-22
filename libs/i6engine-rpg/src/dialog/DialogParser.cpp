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

#include "i6engine/rpg/dialog/DialogParser.h"

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/rpg/dialog/Dialog.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6e {
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
} /* namespace i6e */
