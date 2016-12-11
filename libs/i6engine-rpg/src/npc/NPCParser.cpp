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

#include "i6engine/rpg/npc/NPCParser.h"

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6e {
namespace rpg {
namespace npc {

	NPCParser::NPCParser() : utils::Singleton<NPCParser>(), _npcs(), _npcFiles() {
	}

	NPCParser::~NPCParser() {
	}

	void NPCParser::loadNPCs(const std::string & directory) {
		_npcs.clear();
		_npcFiles.clear();

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
						loadNPC(file);
					} else if (boost::filesystem::is_directory(*iter)) {
						std::string path = iter->path().string();
						directories.push(path);
					}
					iter++;
				}
			} catch (boost::filesystem::filesystem_error & e) {
				ISIXE_THROW_FAILURE("NPCParser", e.what());
			}
		}
	}

	api::objects::GOTemplate NPCParser::getTemplate(const std::string & identifier) const {
		auto it = _npcs.find(identifier);
		if (it == _npcs.end()) {
			ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' not found!");
		}
		return it->second;
	}

	void NPCParser::loadNPC(const std::string & file) {
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("NPCParser", "Couldn't open NPC file (" << file << ")");
			return;
		}

		for (tinyxml2::XMLElement * npc = doc.FirstChildElement("NPC"); npc != nullptr; npc = npc->NextSiblingElement("NPC")) {
			api::objects::GOTemplate tpl;
			if (npc->Attribute("identifier") == nullptr) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC in file '" << file << "' misses identifier!");
			}
			std::string identifier = npc->Attribute("identifier");
			if (npc->FirstChildElement("Name") == nullptr) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' in file '" << file << "' misses Name!");
			}
			std::string name = npc->FirstChildElement("Name")->GetText();
			if (npc->FirstChildElement("XP") == nullptr) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' in file '" << file << "' misses XP!");
			}
			std::string xp = npc->FirstChildElement("XP")->GetText();
			if (npc->FirstChildElement("NextXP") == nullptr) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' in file '" << file << "' misses NextXP!");
			}
			std::string nextXP = npc->FirstChildElement("NextXP")->GetText();
			if (npc->FirstChildElement("Level") == nullptr) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' in file '" << file << "' misses Level!");
			}
			std::string level = npc->FirstChildElement("Level")->GetText();
			if (npc->FirstChildElement("Model") == nullptr) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' in file '" << file << "' misses Nodel!");
			}
			std::string model = npc->FirstChildElement("Model")->GetText();

			api::attributeMap paramsMeshAppearance;
			paramsMeshAppearance.insert(std::make_pair("mesh", model));
			tpl._components.push_back(api::objects::GOTemplateComponent("MeshAppearance", paramsMeshAppearance, "", false, false));

			api::attributeMap paramsName;
			paramsName.insert(std::make_pair("name", name));
			tpl._components.push_back(api::objects::GOTemplateComponent("Name", paramsName, "", false, false));

			api::attributeMap paramsExperience;
			paramsExperience.insert(std::make_pair("currentXP", xp));
			paramsExperience.insert(std::make_pair("nextXP", nextXP));
			paramsExperience.insert(std::make_pair("level", level));
			tpl._components.push_back(api::objects::GOTemplateComponent("Experience", paramsExperience, "", false, false));

			// optional stuff
			// attributes
			api::attributeMap paramsAttribute;
			if (npc->FirstChildElement("HP") != nullptr) {
				paramsAttribute.insert(std::make_pair("attribute_0", npc->FirstChildElement("HP")->GetText()));
			}
			if (npc->FirstChildElement("MaxHP") != nullptr) {
				paramsAttribute.insert(std::make_pair("attribute_1", npc->FirstChildElement("MaxHP")->GetText()));
			}
			tpl._components.push_back(api::objects::GOTemplateComponent("Attribute", paramsAttribute, "", false, false));

			if (_npcs.find(identifier) != _npcs.end()) {
				ISIXE_THROW_FAILURE("NPCParser", "NPC with identifier '" << identifier << "' already defined! Second definition in file '" << file << "'");
			}

			tpl._type = "NPC";
			_npcs.insert(std::make_pair(identifier, tpl));
			_npcFiles.push_back(std::make_pair(identifier, file));
		}
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
