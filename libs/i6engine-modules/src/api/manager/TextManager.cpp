#include "i6engine/api/manager/TextManager.h"

#include "i6engine/utils/Exceptions.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6engine {
namespace api {

	TextManager::TextManager() : _language("Deutsch"), _texts() {
	}

	void TextManager::initialize(const std::string & textDir) {
		boost::filesystem::directory_iterator iter(textDir), dirEnd;
		while (iter != dirEnd) {
			if (boost::filesystem::is_regular_file(*iter)) {
				std::string file = iter->path().string();
				loadFile(file);
			}
			iter++;
		}
	}

	void TextManager::loadFile(const std::string & file) {
		tinyxml2::XMLDocument doc;

		tinyxml2::XMLError e = doc.LoadFile(file.c_str());

		if (e) {
			ISIXE_THROW_API("TextManager", "Couldn't open xml file!");
			return;
		}

		for (tinyxml2::XMLElement * node = doc.FirstChildElement("Key"); node != nullptr; node = node->NextSiblingElement("Key")) {
			std::string key = node->Attribute("key");

			for (tinyxml2::XMLElement * language = node->FirstChildElement("Language"); language != nullptr; language = language->NextSiblingElement("Language")) {
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

				_texts[language->Attribute("name")].insert(std::make_pair(key, addText));
			}
		}
	}

	std::string TextManager::getText(const std::string & key) const {
		auto it = _texts.find(_language);

		if (it == _texts.end()) {
			ISIXE_THROW_API("TextManager", "No text for key '" << key << "' with language '" << _language << "'");
		}

		auto it2 = it->second.find(key);

		if (it2 == it->second.end()) {
			ISIXE_THROW_API("TextManager", "No text for key '" << key << "' with language '" << _language << "'");
		}

		return it2->second;
	}

} /* namespace api */
} /* namespace i6engine */
