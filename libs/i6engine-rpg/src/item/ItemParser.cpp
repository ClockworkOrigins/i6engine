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

#include "i6engine/rpg/item/ItemParser.h"

#include <queue>

#include "i6engine/utils/Exceptions.h"

#include "boost/filesystem.hpp"

#include "tinyxml2.h"

namespace i6e {
namespace rpg {
namespace item {

	ItemParser::ItemParser() : _items(), _itemAttributes(), _itemTypes() {
	}

	ItemParser::~ItemParser() {
	}

	void ItemParser::loadItems(const std::string & directory) {
		_items.clear();
		_itemAttributes.clear();
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
						loadItem(file);
					} else if (boost::filesystem::is_directory(*iter)) {
						std::string path = iter->path().string();
						directories.push(path);
					}
					iter++;
				}
			} catch (boost::filesystem::filesystem_error & e) {
				ISIXE_THROW_FAILURE("ItemParser", e.what());
			}
		}
	}

	std::pair<std::string, api::objects::GOTemplate> ItemParser::getTemplate(const std::string & identifier) const {
		auto it = _items.find(identifier);
		if (it == _items.end()) {
			ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' not found!");
		}
		return it->second;
	}

	void ItemParser::loadItem(const std::string & file) {
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(file.c_str())) {
			ISIXE_THROW_FAILURE("ItemParser", "Couldn't open Item file (" << file << ")");
			return;
		}

		for (tinyxml2::XMLElement * item = doc.FirstChildElement("Item"); item != nullptr; item = item->NextSiblingElement("Item")) {
			api::objects::GOTemplate tpl;
			if (item->Attribute("type") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item in file '" << file << "' misses type!");
			}
			std::string type = item->Attribute("type");
			if (item->Attribute("identifier") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item in file '" << file << "' misses identifier!");
			}
			std::string identifier = item->Attribute("identifier");
			if (item->FirstChildElement("Name") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' in file '" << file << "' misses Name!");
			}
			std::string name = item->FirstChildElement("Name")->GetText();
			if (item->FirstChildElement("Mesh") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' in file '" << file << "' misses Mesh!");
			}
			std::string mesh = item->FirstChildElement("Mesh")->GetText();
			if (item->FirstChildElement("Imageset") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' in file '" << file << "' misses Imageset!");
			}
			std::string imageset = item->FirstChildElement("Imageset")->GetText();
			if (item->FirstChildElement("Image") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' in file '" << file << "' misses Image!");
			}
			std::string image = item->FirstChildElement("Image")->GetText();
			if (item->FirstChildElement("Value") == nullptr) {
				ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' in file '" << file << "' misses Value!");
			}
			std::string value = item->FirstChildElement("Value")->GetText();

			api::attributeMap paramsName;
			paramsName.insert(std::make_pair("name", name));
			tpl._components.push_back(api::objects::GOTemplateComponent("Name", paramsName, "", false, false));

			api::attributeMap paramsMesh;
			paramsMesh.insert(std::make_pair("mesh", mesh));
			tpl._components.push_back(api::objects::GOTemplateComponent("MeshAppearance", paramsMesh, "", false, false));

			api::attributeMap paramsItem;
			paramsItem.insert(std::make_pair("ident", identifier));
			paramsItem.insert(std::make_pair("imageset", imageset));
			paramsItem.insert(std::make_pair("image", imageset));
			paramsItem.insert(std::make_pair("value", value));

			if (item->FirstChildElement("Weight") != nullptr) {
				paramsItem.insert(std::make_pair("weight", item->FirstChildElement("Weight")->GetText()));
			}
			if (item->FirstChildElement("SlotWidth") != nullptr && item->FirstChildElement("SlotHeight") != nullptr) {
				api::attributeMap paramsSlot;
				paramsSlot.insert(std::make_pair("width", item->FirstChildElement("SlotWidth")->GetText()));
				paramsSlot.insert(std::make_pair("height", item->FirstChildElement("SlotHeight")->GetText()));
				tpl._components.push_back(api::objects::GOTemplateComponent("Slot", paramsSlot, "", false, false));
			}

			auto it = _itemAttributes.find(type);
			if (it != _itemAttributes.end()) {
				for (auto & p : it->second) {
					if (item->FirstChildElement(p.first.c_str()) != nullptr) {
						paramsItem.insert(std::make_pair(p.second, item->FirstChildElement(p.first.c_str())->GetText()));
					}
				}
			}

			tpl._components.push_back(api::objects::GOTemplateComponent(type, paramsItem, "", false, false));

			_itemTypes.insert(type);

			if (_items.find(identifier) != _items.end()) {
				ISIXE_THROW_FAILURE("ItemParser", "Item with identifier '" << identifier << "' already defined! Second definition in file '" << file << "'");
			}

			_items.insert(std::make_pair(identifier, std::make_pair(type, tpl)));
		}
	}

} /* namespace item */
} /* namespace rpg */
} /* namespace i6e */
