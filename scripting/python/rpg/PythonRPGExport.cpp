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

#include "boost/python.hpp"

namespace i6e {
namespace python {
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
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingRPGPython) {
	using namespace boost::python;

	def("insertItemAtPosition", &i6e::python::rpg::insertItemAtPosition);
	def("insertItemAtWaypoint", &i6e::python::rpg::insertItemAtWaypoint);
	def("insertNPCAtPosition", &i6e::python::rpg::insertNPCAtPosition);
	def("insertNPCAtWaypoint", &i6e::python::rpg::insertNPCAtWaypoint);
	def("insertPlayerAtWaypoint", &i6e::python::rpg::insertPlayerAtWaypoint);
	def("getNPC", &i6e::python::rpg::getNPC, return_internal_reference<>());
	def("wasHeard", &i6e::python::rpg::wasHeard);
	def("exitDialog", &i6e::python::rpg::exitDialog);
	def("isDialogRunning", &i6e::python::rpg::isDialogRunning);

	class_<i6e::rpg::dialog::Dialog>("Dialog", no_init);
	class_<i6e::rpg::npc::NPC>("NPC", no_init)
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
		.add_property("xp", &i6e::rpg::npc::NPC::getCurrentXP, &i6e::rpg::npc::NPC::setCurrentXP)
		.add_property("nextXP", &i6e::rpg::npc::NPC::getNextXP, &i6e::rpg::npc::NPC::setNextXP)
		.add_property("level", &i6e::rpg::npc::NPC::getLevel, &i6e::rpg::npc::NPC::setLevel)
		.def("createItems", &i6e::rpg::npc::NPC::createItems)
		.def("removeItems", &i6e::rpg::npc::NPC::removeItems)
		.def("trade", &i6e::rpg::npc::NPC::trade);

	enum_<i6e::rpg::quest::QuestStatus>("QuestStatus")
		.value("LOCKED", i6e::rpg::quest::QuestStatus::LOCKED)
		.value("RUNNING", i6e::rpg::quest::QuestStatus::RUNNING)
		.value("COMPLETED", i6e::rpg::quest::QuestStatus::COMPLETED)
		.value("FAILED", i6e::rpg::quest::QuestStatus::FAILED)
		.export_values();

	def("setQuestStatus", &i6e::python::rpg::setQuestStatus);
	def("addLogEntry", &i6e::python::rpg::addLogEntry);
	def("getItemName", &i6e::python::rpg::getItemName);
	def("getQuestName", &i6e::python::rpg::getQuestName);
}
