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

#include "i6engine/rpg/quest/Quest.h"
#include "i6engine/rpg/quest/QuestLog.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace rpg {

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
		i6engine::rpg::item::ItemManager::GetSingletonPtr()->createItem(identifier, pos, rot);
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
		i6engine::rpg::npc::NPCManager::GetSingletonPtr()->createNPC(identifier, pos, rot, false);
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
		i6engine::rpg::npc::NPCManager::GetSingletonPtr()->createNPC(identifier, pos, rot, true);
	}

	i6engine::rpg::npc::NPC * getNPC(const std::string & identifier) {
		return i6engine::rpg::npc::NPCManager::GetSingleton().getNPC(identifier);
	}

	bool wasHeard(const std::string & identifier) {
		return i6engine::rpg::dialog::DialogManager::GetSingletonPtr()->wasHeard(identifier);
	}

	void exitDialog() {
		i6engine::rpg::dialog::DialogManager::GetSingletonPtr()->exitDialog();
	}

	void isDialogRunning() {
		i6engine::rpg::dialog::DialogManager::GetSingletonPtr()->isDialogRunning();
	}

	void setQuestStatus(const std::string & identifier, i6engine::rpg::quest::QuestStatus status) {
		i6engine::rpg::quest::QuestLog::GetSingleton().setQuestStatus(identifier, status);
	}

	void addLogEntry(const std::string & identifier, const std::string & entry) {
		i6engine::rpg::quest::QuestLog::GetSingleton().addLogEntry(identifier, entry);
	}

} /* namespace rpg */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingRPGPython) {
	using namespace boost::python;

	def("insertItemAtWaypoint", &i6engine::python::rpg::insertItemAtWaypoint);
	def("insertNPCAtWaypoint", &i6engine::python::rpg::insertNPCAtWaypoint);
	def("insertPlayerAtWaypoint", &i6engine::python::rpg::insertPlayerAtWaypoint);
	def("getNPC", &i6engine::python::rpg::getNPC, return_internal_reference<>());
	def("wasHeard", &i6engine::python::rpg::wasHeard);
	def("exitDialog", &i6engine::python::rpg::exitDialog);
	def("isDialogRunning", &i6engine::python::rpg::isDialogRunning);

	class_<i6engine::rpg::dialog::Dialog>("Dialog", no_init);
	class_<i6engine::rpg::npc::NPC>("NPC", no_init)
		.def("getGO", &i6engine::rpg::npc::NPC::getGO)
		.def("turnToNPC", &i6engine::rpg::npc::NPC::turnToNPC)
		.def("say", &i6engine::rpg::npc::NPC::say)
		.def("getItemCount", &i6engine::rpg::npc::NPC::getItemCount)
		.def("addJob", &i6engine::rpg::npc::NPC::addJob)
		.def("setCurrentXP", &i6engine::rpg::npc::NPC::setCurrentXP)
		.def("getCurrentXP", &i6engine::rpg::npc::NPC::getCurrentXP)
		.def("setNextXP", &i6engine::rpg::npc::NPC::setNextXP)
		.def("getNextXP", &i6engine::rpg::npc::NPC::getNextXP)
		.def("setLevel", &i6engine::rpg::npc::NPC::setLevel)
		.def("getLevel", &i6engine::rpg::npc::NPC::getLevel)
		.add_property("xp", &i6engine::rpg::npc::NPC::getCurrentXP, &i6engine::rpg::npc::NPC::setCurrentXP)
		.add_property("nextXP", &i6engine::rpg::npc::NPC::getNextXP, &i6engine::rpg::npc::NPC::setNextXP)
		.add_property("level", &i6engine::rpg::npc::NPC::getLevel, &i6engine::rpg::npc::NPC::setLevel);

	enum_<i6engine::rpg::quest::QuestStatus>("QuestStatus")
		.value("LOCKED", i6engine::rpg::quest::QuestStatus::LOCKED)
		.value("RUNNING", i6engine::rpg::quest::QuestStatus::RUNNING)
		.value("COMPLETED", i6engine::rpg::quest::QuestStatus::COMPLETED)
		.value("FAILED", i6engine::rpg::quest::QuestStatus::FAILED)
		.export_values();

	def("setQuestStatus", &i6engine::python::rpg::setQuestStatus);
	def("addLogEntry", &i6engine::python::rpg::addLogEntry);
}
