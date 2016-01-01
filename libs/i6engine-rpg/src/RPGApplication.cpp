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

#include "i6engine/rpg/RPGApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
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

#include "i6engine/rpg/gui/SubtitleWidget.h"

#include "i6engine/rpg/dialog/DialogManager.h"

#include "i6engine/rpg/item/ItemManager.h"

#include "i6engine/rpg/npc/NPCParser.h"

#include "i6engine/rpg/quest/QuestLog.h"

#include "boost/bind.hpp"
#include "boost/functional/factory.hpp"

namespace i6engine {
namespace rpg {

	RPGApplication::RPGApplication() : api::Application(), _iniParser() {
		_iniParser.load("RPG.ini");
	}

	RPGApplication::~RPGApplication() {
	}

	void RPGApplication::Initialize() {
		std::string externalConstantsFile;
		if (_iniParser.getValue("SCRIPT", "externalConstants", externalConstantsFile) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'externalConstants' in section 'SCRIPT' in RPG.ini not found!");
		}
		config::parseExternalConstants(externalConstantsFile);

		item::ItemManager::GetSingletonPtr()->addItemAttributes("UsableItem", { { "HP", "attribute_0" }, { "HP_MAX", "attribute_1" } });

		std::string ItemDirectory;
		if (_iniParser.getValue("SCRIPT", "itemDirectory", ItemDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'itemDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		item::ItemManager::GetSingletonPtr()->loadItems(ItemDirectory);

		std::string NPCDirectory;
		if (_iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'npcDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		npc::NPCParser::GetSingletonPtr()->loadNPCs(NPCDirectory);

		// Load Dialogs after NPCs because they require them
		std::string DialogDirectory;
		if (_iniParser.getValue("SCRIPT", "dialogDirectory", DialogDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'dialogDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		dialog::DialogManager::GetSingletonPtr()->loadDialogs(DialogDirectory);

		// Load Quests
		std::string QuestDirectory;
		if (_iniParser.getValue("SCRIPT", "questDirectory", QuestDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'questDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		quest::QuestLog::GetSingletonPtr()->loadQuests(QuestDirectory);
	}

	void RPGApplication::AfterInitialize() {
		// load all scripts in LUAScriptDir, so all function calls can be done without a file
		api::EngineController::GetSingletonPtr()->getScriptingFacade()->loadAllScripts();

		api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
		gf->registerWidgetTemplate("Subtitle", boost::factory<gui::SubtitleWidget *>());

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// sets gravity for the game... here like on earth
		std::string gravityString;
		_iniParser.getValue("PHYSIC", "gravity", gravityString);
		api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(gravityString));

		// ambient light for the scene
		std::string ambientLightString;
		_iniParser.getValue("GRAPHIC", "ambientLight", ambientLightString);
		auto vec = utils::split(ambientLightString, " ");
		api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(std::stod(vec[0]), std::stod(vec[1]), std::stod(vec[2]));

		// setting shadow technique... currently only additive stencil possible
		uint16_t shadowTechnique;
		_iniParser.getValue("GRAPHIC", "shadowTechnique", shadowTechnique);
		api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(api::graphics::ShadowTechnique(shadowTechnique));

		// setting distance fog
		api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setExponentialFog(Vec3(0.9, 0.9, 0.9), 0.005);

		// register rpg components we want to use
		// do this befor loading the level
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Attribute", boost::bind(&components::AttributeComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("DialogChecker", boost::bind(&components::DialogCheckerComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Experience", boost::bind(&components::ExperienceComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Healthbar", boost::bind(&components::HealthbarComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("HumanMovement", boost::bind(&components::HumanMovementComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ListInventory", boost::bind(&components::ListInventoryComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("MiscItem", boost::bind(&components::MiscItemComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Name", boost::bind(&components::NameComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Quickslot", boost::bind(&components::QuickslotComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Slot", boost::bind(&components::SlotComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("SlotInventory", boost::bind(&components::SlotInventoryComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ThirdPersonControl", boost::bind(&components::ThirdPersonControlComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("UsableItem", boost::bind(&components::UsableItemComponent::createC, _1, _2));
		api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("WeightInventory", boost::bind(&components::WeightInventoryComponent::createC, _1, _2));

		// register keys
		uint16_t key;
		// action key, default: E
		_iniParser.getValue("INPUT", "action", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "action");
		_iniParser.getValue("INPUT", "action2", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "action");
		// inventory key, default: TAB
		_iniParser.getValue("INPUT", "inventory", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "inventory");
		// questLog key, default: J
		_iniParser.getValue("INPUT", "questLog", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "questLog");

		// forward key, default: W
		_iniParser.getValue("INPUT", "forward", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "forward");
		// backward key, default: S
		_iniParser.getValue("INPUT", "backward", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "backward");
		// left key, default: A
		_iniParser.getValue("INPUT", "left", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "left");
		// right key, default: D
		_iniParser.getValue("INPUT", "right", key);
		api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(api::KeyCode(key), "right");

		// game config
		// subtitles
		bool subtitles;
		_iniParser.getValue("GAME", "subtitles", subtitles);
		dialog::DialogManager::GetSingleton().setSubtitlesEnabled(subtitles);

		// subtitle position
		double subtitleBoxPositionX;
		_iniParser.getValue("GAME", "subtitleBoxPositionX", subtitleBoxPositionX);
		double subtitleBoxPositionY;
		_iniParser.getValue("GAME", "subtitleBoxPositionY", subtitleBoxPositionY);
		dialog::DialogManager::GetSingleton().setSubtitlePosition(Vec2(subtitleBoxPositionX, subtitleBoxPositionY));

		// subtitle size
		double subtitleBoxSizeX;
		_iniParser.getValue("GAME", "subtitleBoxSizeX", subtitleBoxSizeX);
		double subtitleBoxSizeY;
		_iniParser.getValue("GAME", "subtitleBoxSizeY", subtitleBoxSizeY);
		dialog::DialogManager::GetSingleton().setSubtitleSize(Vec2(subtitleBoxSizeX, subtitleBoxSizeY));

		// subtitle font
		std::string subtitleFont;
		_iniParser.getValue("GAME", "subtitleFont", subtitleFont);
		dialog::DialogManager::GetSingleton().setSubtitleFont(subtitleFont);

		// dialog numbers
		bool dialogNumbers;
		_iniParser.getValue("GAME", "dialogNumbers", dialogNumbers);
		dialog::DialogManager::GetSingleton().setDialogNumbersEnabled(dialogNumbers);
	}

	void RPGApplication::loadLevel(const std::string & level) const {
		// loads the RPG demo level
		api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(level, "Singleplayer");

		// call Startup in script
#if ISIXE_SCRIPTING != SCRIPTING_NONE
		api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>("Startup");
#endif
	}

	void RPGApplication::Tick() {
	}

	bool RPGApplication::ShutdownRequest() {
		return true;
	}

	void RPGApplication::Finalize() {
	}

	void RPGApplication::ShutDown() {
	}

} /* namespace rpg */
} /* namespace i6engine */
