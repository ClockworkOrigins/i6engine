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

#include "LuaRPGExport.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/WaypointComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include "i6engine/rpg/item/ItemManager.h"

#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/NPCManager.h"
#include "i6engine/rpg/npc/NPCQueueJob.h"

#include "i6engine/rpg/quest/Quest.h"
#include "i6engine/rpg/quest/QuestLog.h"

namespace i6e {
namespace lua {
namespace rpg {

	void insertItemAtPosition(const std::string & identifier, const Vec3 & pos, const Quaternion & rot) {
		i6e::rpg::item::ItemManager::GetSingletonPtr()->createItem(identifier, pos, rot);
	}

	void insertItemAtWaypoint(const std::string & identifier, const std::string & waypoint) {
		Vec3 pos;
		Quaternion rot;
		for (auto & go : api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint")) {
			if (go->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent)->getName() == waypoint) {
				pos = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getPosition();
				rot = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getRotation();
				break;
			}
		}
		insertItemAtPosition(identifier, pos, rot);
	}

	void insertNPCAtPosition(const std::string & identifier, const Vec3 & pos, const Quaternion & rot) {
		i6e::rpg::npc::NPCManager::GetSingletonPtr()->createNPC(identifier, pos, rot, false);
	}

	void insertNPCAtWaypoint(const std::string & identifier, const std::string & waypoint) {
		Vec3 pos;
		Quaternion rot;
		for (auto & go : api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint")) {
			if (go->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent)->getName() == waypoint) {
				pos = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getPosition();
				rot = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getRotation();
				break;
			}
		}
		insertNPCAtPosition(identifier, pos, rot);
	}

	void insertPlayerAtWaypoint(const std::string & identifier, const std::string & waypoint) {
		Vec3 pos;
		Quaternion rot;
		for (auto & go : api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint")) {
			if (go->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent)->getName() == waypoint) {
				pos = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getPosition();
				rot = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent)->getRotation();
				break;
			}
		}
		i6e::rpg::npc::NPCManager::GetSingletonPtr()->createNPC(identifier, pos, rot, true);
	}

	i6e::rpg::npc::NPC * getNPC(const std::string & identifier) {
		return i6e::rpg::npc::NPCManager::GetSingleton().getNPC(identifier);
	}

	bool wasHeard(const std::string & identifier) {
		return i6e::rpg::dialog::DialogManager::GetSingletonPtr()->wasHeard(identifier);
	}

	void exitDialog() {
		i6e::rpg::dialog::DialogManager::GetSingletonPtr()->exitDialog();
	}

	void isDialogRunning() {
		i6e::rpg::dialog::DialogManager::GetSingletonPtr()->isDialogRunning();
	}

	void setQuestStatus(const std::string & identifier, i6e::rpg::quest::QuestStatus status) {
		i6e::rpg::quest::QuestLog::GetSingleton().setQuestStatus(identifier, status);
	}

	void addLogEntry(const std::string & identifier, const std::string & entry) {
		i6e::rpg::quest::QuestLog::GetSingleton().addLogEntry(identifier, entry);
	}

	std::string getItemName(const std::string & identifier) {
		return i6e::rpg::item::ItemManager::GetSingleton().getItemName(identifier);
	}

	std::string getQuestName(const std::string & identifier) {
		return i6e::rpg::quest::QuestLog::GetSingleton().getQuestName(identifier);
	}

} /* namespace rpg */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerRPG() {
	return
		def("insertItemAtPosition", &i6e::lua::rpg::insertItemAtPosition),
		def("insertItemAtWaypoint", &i6e::lua::rpg::insertItemAtWaypoint),
		def("insertNPCAtPosition", &i6e::lua::rpg::insertNPCAtPosition),
		def("insertNPCAtWaypoint", &i6e::lua::rpg::insertNPCAtWaypoint),
		def("insertPlayerAtWaypoint", &i6e::lua::rpg::insertPlayerAtWaypoint),
		def("getNPC", &i6e::lua::rpg::getNPC),
		def("wasHeard", &i6e::lua::rpg::wasHeard),
		def("exitDialog", &i6e::lua::rpg::exitDialog),
		def("isDialogRunning", &i6e::lua::rpg::isDialogRunning),

		class_<i6e::rpg::dialog::Dialog>("Dialog")
			.def(constructor<const std::string &, const std::string &>()),

		class_<i6e::rpg::npc::NPC>("NPC")
			.def(constructor<const i6e::api::objects::GOTemplate &, bool>())
			.def("getGO", &i6e::rpg::npc::NPC::getGO)
			.def("turnToNPC", &i6e::rpg::npc::NPC::turnToNPC)
			.def("say", &i6e::rpg::npc::NPC::say)
			.def("getItemCount", &i6e::rpg::npc::NPC::getItemCount)
			.def("addJob", &i6e::rpg::npc::NPC::addJob)
			.def("setCurrentXP", &i6e::rpg::npc::NPC::setCurrentXP)
			.def("getCurrentXP", &i6e::rpg::npc::NPC::getCurrentXP)
			.def("setNextXP", &i6e::rpg::npc::NPC::setNextXP)
			.def("getNextXP", &i6e::rpg::npc::NPC::getNextXP)
			.def("setLevel", &i6e::rpg::npc::NPC::setLevel)
			.def("getLevel", &i6e::rpg::npc::NPC::getLevel)
			.property("xp", &i6e::rpg::npc::NPC::getCurrentXP, &i6e::rpg::npc::NPC::setCurrentXP)
			.property("nextXP", &i6e::rpg::npc::NPC::getNextXP, &i6e::rpg::npc::NPC::setNextXP)
			.property("level", &i6e::rpg::npc::NPC::getLevel, &i6e::rpg::npc::NPC::setLevel)
			.def("createItems", &i6e::rpg::npc::NPC::createItems)
			.def("removeItems", &i6e::rpg::npc::NPC::removeItems)
			.def("trade", &i6e::rpg::npc::NPC::trade),

		class_<i6e::rpg::quest::QuestStatus>("QuestStatus")
			.def(constructor<>())
			.enum_("QuestStatus")
			[
				value("LOCKED", int(i6e::rpg::quest::QuestStatus::LOCKED)),
				value("RUNNING", int(i6e::rpg::quest::QuestStatus::RUNNING)),
				value("COMPLETED", int(i6e::rpg::quest::QuestStatus::COMPLETED)),
				value("FAILED", int(i6e::rpg::quest::QuestStatus::FAILED))
			],

		def("setQuestStatus", &i6e::lua::rpg::setQuestStatus),
		def("addLogEntry", &i6e::lua::rpg::addLogEntry),
		def("getItemName", &i6e::lua::rpg::getItemName),
		def("getQuestName", &i6e::lua::rpg::getQuestName)
		;
}
