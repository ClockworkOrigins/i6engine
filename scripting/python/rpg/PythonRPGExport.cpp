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

#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/NPCManager.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace rpg {

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

} /* namespace rpg */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingRPGPython) {
	using namespace boost::python;

	def("insertNPCAtWaypoint", &i6engine::python::rpg::insertNPCAtWaypoint);
	def("insertPlayerAtWaypoint", &i6engine::python::rpg::insertPlayerAtWaypoint);
	def("getNPC", &i6engine::python::rpg::getNPC, return_internal_reference<>());
	def("wasHeard", &i6engine::python::rpg::wasHeard);
	def("exitDialog", &i6engine::python::rpg::exitDialog);

	class_<i6engine::rpg::dialog::Dialog>("Dialog", no_init);
	class_<i6engine::rpg::npc::NPC>("NPC", no_init)
		.def("getGO", &i6engine::rpg::npc::NPC::getGO)
		.def("turnToNPC", &i6engine::rpg::npc::NPC::turnToNPC)
		.def("say", &i6engine::rpg::npc::NPC::say);
}
