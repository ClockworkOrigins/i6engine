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

#include "EditorRPGInitialization.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/rpg/components/AttributeComponent.h"
#include "i6engine/rpg/components/DialogCheckerComponent.h"
#include "i6engine/rpg/components/ExperienceComponent.h"
#include "i6engine/rpg/components/HealthbarComponent.h"
#include "i6engine/rpg/components/HumanMovementComponent.h"
#include "i6engine/rpg/components/ListInventoryComponent.h"
#include "i6engine/rpg/components/MiscItemComponent.h"
#include "i6engine/rpg/components/NameComponent.h"
#include "i6engine/rpg/components/QuickslotComponent.h"
#include "i6engine/rpg/components/SlotComponent.h"
#include "i6engine/rpg/components/SlotInventoryComponent.h"
#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
#include "i6engine/rpg/components/UsableItemComponent.h"
#include "i6engine/rpg/components/WeightInventoryComponent.h"

#include "i6engine/rpg/config/ExternalConstants.h"
#include "i6engine/rpg/dialog/DialogManager.h"
#include "i6engine/rpg/item/ItemManager.h"
#include "i6engine/rpg/npc/NPCParser.h"
#include "i6engine/rpg/quest/QuestLog.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6e {
namespace plugins {

	void EditorRPGInitialization::initialize() {
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load("RPG.ini");

		std::string externalConstantsFile;
		if (iniParser.getValue("SCRIPT", "externalConstants", externalConstantsFile) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'externalConstants' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::config::parseExternalConstants(externalConstantsFile);

		rpg::item::ItemManager::GetSingletonPtr()->addItemAttributes("UsableItem", { { "HP", "attribute_0" }, { "HP_MAX", "attribute_1" } });

		std::string ItemDirectory;
		if (iniParser.getValue("SCRIPT", "itemDirectory", ItemDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'itemDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::item::ItemManager::GetSingletonPtr()->loadItems(ItemDirectory);

		std::string NPCDirectory;
		if (iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'npcDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::npc::NPCParser::GetSingletonPtr()->loadNPCs(NPCDirectory);

		// Load Dialogs after NPCs because they require them
		std::string DialogDirectory;
		if (iniParser.getValue("SCRIPT", "dialogDirectory", DialogDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'dialogDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::dialog::DialogManager::GetSingletonPtr()->loadDialogs(DialogDirectory);

		// Load Quests
		std::string QuestDirectory;
		if (iniParser.getValue("SCRIPT", "questDirectory", QuestDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'questDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::quest::QuestLog::GetSingletonPtr()->loadQuests(QuestDirectory);

		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Attribute", boost::bind(api::Component::createC<rpg::components::AttributeComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("DialogChecker", boost::bind(api::Component::createC<rpg::components::DialogCheckerComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Experience", boost::bind(api::Component::createC<rpg::components::ExperienceComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Healthbar", boost::bind(api::Component::createC<rpg::components::HealthbarComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("HumanMovement", boost::bind(api::Component::createC<rpg::components::HumanMovementComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ListInventory", boost::bind(api::Component::createC<rpg::components::ListInventoryComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("MiscItem", boost::bind(api::Component::createC<rpg::components::MiscItemComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Name", boost::bind(api::Component::createC<rpg::components::NameComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Quickslot", boost::bind(api::Component::createC<rpg::components::QuickslotComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Slot", boost::bind(api::Component::createC<rpg::components::SlotComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("SlotInventory", boost::bind(api::Component::createC<rpg::components::SlotInventoryComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ThirdPersonControl", boost::bind(api::Component::createC<rpg::components::ThirdPersonControlComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("UsableItem", boost::bind(api::Component::createC<rpg::components::UsableItemComponent>, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("WeightInventory", boost::bind(api::Component::createC<rpg::components::WeightInventoryComponent>, _1, _2));
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.initialization.InitializationPluginInterface")
