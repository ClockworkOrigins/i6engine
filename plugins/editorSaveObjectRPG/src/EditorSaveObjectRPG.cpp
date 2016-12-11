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

#include "EditorSaveObjectRPG.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/DialogCheckerComponent.h"
#include "i6engine/rpg/components/ItemComponent.h"
#include "i6engine/rpg/item/ItemManager.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6e {
namespace plugins {

	EditorSaveObjectRPG::EditorSaveObjectRPG() : _file(), _scriptsPath(), _fileExtension(), _level() {
		std::string scriptsPath;
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		if (clockUtils::ClockError::SUCCESS != i6eEngineController->getIniParser().getValue("SCRIPT", "LuaScriptsPath", scriptsPath)) {
			ISIXE_THROW_FAILURE("EditorSaveObjectRPG", "An exception has occurred: value LuaScriptsPath in section SCRIPT not found!");
		}
		_fileExtension = ".lua";
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		if (clockUtils::ClockError::SUCCESS != i6eEngineController->getIniParser().getValue("SCRIPT", "PythonScriptsPath", scriptsPath)) {
			ISIXE_THROW_FAILURE("EditorSaveObjectRPG", "An exception has occurred: value PythonScriptsPath in section SCRIPT not found!");
		}
		_fileExtension = ".py";
#endif
		_scriptsPath = QString::fromStdString(scriptsPath);
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load("RPG.ini");
		std::string ItemDirectory;
		if (iniParser.getValue("SCRIPT", "itemDirectory", ItemDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'itemDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::item::ItemManager::GetSingletonPtr()->loadItems(ItemDirectory);
	}

	void EditorSaveObjectRPG::startLevel(const QString & level) {
		QStringList vec = level.split("/");
		vec = vec.back().split("\\");
		QString levelStartup = vec.back().split(".").front();
		_file.close();
		_file.open((_scriptsPath + "/Startup_" + levelStartup + _fileExtension).toStdString().c_str());
		_level = levelStartup;
		insertMethodDeclaration();
	}

	bool EditorSaveObjectRPG::saveObject(const i6e::api::GOPtr & go, tinyxml2::XMLElement *) {
		bool handled = false;
		if (go->getType() == "NPC") {
			saveNPC(go->getGOC<rpg::components::DialogCheckerComponent>(rpg::components::config::DialogCheckerComponent)->getNPCIdentifier(), go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getPosition(), go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent)->getRotation());
			handled = true;
		}
		// check items
		for (std::string s : rpg::item::ItemManager::GetSingletonPtr()->getItemTypes()) {
			if (go->getType() == s) {
				saveItem(go->getGOC<rpg::components::ItemComponent>(rpg::components::config::ComponentTypes::ItemComponent)->getItemIdentifier(), go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getPosition(), go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getRotation());
				handled = true;
				break;
			}
		}
		return handled;
	}

	void EditorSaveObjectRPG::finishLevel() {
		insertMethodEnd();
		_file.flush();
		_file.close();
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.saveObject.SaveObjectPluginInterface")
