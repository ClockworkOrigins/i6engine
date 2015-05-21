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

#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/NPCManager.h"

namespace i6engine {
namespace lua {
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
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerRPG() {
	return
		def("insertNPCAtWaypoint", &i6engine::lua::rpg::insertNPCAtWaypoint),
		def("insertPlayerAtWaypoint", &i6engine::lua::rpg::insertPlayerAtWaypoint),
		def("getNPC", &i6engine::lua::rpg::getNPC),
		def("wasHeard", &i6engine::lua::rpg::wasHeard),
		def("exitDialog", &i6engine::lua::rpg::exitDialog),

		class_<i6engine::rpg::dialog::Dialog>("Dialog")
			.def(constructor<const std::string &, const std::string &>()),

		class_<i6engine::rpg::npc::NPC>("NPC")
			.def(constructor<const i6engine::api::objects::GOTemplate &, bool>())
			.def("getGO", &i6engine::rpg::npc::NPC::getGO)
			.def("turnToNPC", &i6engine::rpg::npc::NPC::turnToNPC)
			.def("say", &i6engine::rpg::npc::NPC::say)
		;
}
