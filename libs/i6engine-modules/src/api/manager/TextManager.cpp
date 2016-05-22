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

#include "i6engine/api/manager/TextManager.h"

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6e {
namespace api {

	TextManager::TextManager() : _language("Deutsch"), _texts(), _textDir() {
	}

	void TextManager::initialize(const std::string & textDir) {
		_textDir = textDir;
		std::queue<std::string> directories;
		directories.push(textDir);

		while (!directories.empty()) {
			std::string dir = directories.front();
			directories.pop();
			try {
				boost::filesystem::directory_iterator iter(dir), dirEnd;
				while (iter != dirEnd) {
					if (boost::filesystem::is_regular_file(*iter)) {
						std::string file = iter->path().string();
						loadFile(file);
					} else if (boost::filesystem::is_directory(*iter)) {
						std::string path = iter->path().string();
						directories.push(path);
					}
					iter++;
				}
			} catch (boost::filesystem::filesystem_error & e) {
				ISIXE_THROW_FAILURE("TextManager", e.what());
			}
		}
	}

	void TextManager::loadFile(const std::string & file) {
		tinyxml2::XMLDocument doc;

		tinyxml2::XMLError e = doc.LoadFile(file.c_str());

		if (e) {
			ISIXE_THROW_API("TextManager", "Couldn't open file '" << file << "'. Maybe not a valid XML file?");
			return;
		}

		for (tinyxml2::XMLElement * node = doc.FirstChildElement("Key"); node != nullptr; node = node->NextSiblingElement("Key")) {
			std::string key = node->Attribute("key");

			for (tinyxml2::XMLElement * language = node->FirstChildElement("Language"); language != nullptr; language = language->NextSiblingElement("Language")) {
				if (_language != language->Attribute("name")) {
					continue;
				}
				std::string text = language->GetText();

				std::string addText = "";

				for (size_t i = 0; i < text.length(); i++) {
					if (int(text[i]) == -61) {
						if (int(text[i + 1] == -92)) {
							addText += 'ä';
						} else if (int(text[i + 1] == -74)) {
							addText += 'ö';
						} else if (int(text[i + 1] == -68)) {
							addText += 'ü';
						} else if (int(text[i + 1] == -124)) {
							addText += 'Ä';
						} else if (int(text[i + 1] == -106)) {
							addText += 'Ö';
						} else if (int(text[i + 1] == -100)) {
							addText += 'Ü';
						} else if (int(text[i + 1] == -97)) {
							addText += 'ß';
						} else {
							ISIXE_LOG_DEBUG("TextManager", key << " " << int(text[i + 1]));
						}
						i++;
					} else {
						addText += text[i];
					}
				}

				_texts.insert(std::make_pair(key, addText));
			}
		}
	}

	std::string TextManager::getText(const std::string & key) const {
		auto it = _texts.find(key);

		if (it == _texts.end()) {
			ISIXE_THROW_API("TextManager", "No text for key '" << key << "' with language '" << _language << "'");
		}

		return it->second;
	}

} /* namespace api */
} /* namespace i6e */
