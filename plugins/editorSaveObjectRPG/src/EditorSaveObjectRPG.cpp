#include "EditorSaveObjectRPG.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/DialogCheckerComponent.h"
#include "i6engine/rpg/components/ItemComponent.h"
#include "i6engine/rpg/item/ItemManager.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6engine {
namespace plugins {

	EditorSaveObjectRPG::EditorSaveObjectRPG() : _file(), _scriptsPath(), _fileExtension(), _level() {
		std::string scriptsPath;
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "LuaScriptsPath", scriptsPath)) {
			ISIXE_THROW_FAILURE("EditorSaveObjectRPG", "An exception has occurred: value LuaScriptsPath in section SCRIPT not found!");
		}
		_fileExtension = ".lua";
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue("SCRIPT", "PythonScriptsPath", scriptsPath)) {
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
		_file = std::ofstream((_scriptsPath + "/" + levelStartup + _fileExtension).toStdString().c_str());
		_level = levelStartup;
		insertMethodDeclaration();
	}

	bool EditorSaveObjectRPG::saveObject(const i6engine::api::GOPtr & go, tinyxml2::XMLElement *) {
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
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine.editor.saveObject.SaveObjectPluginInterface")
