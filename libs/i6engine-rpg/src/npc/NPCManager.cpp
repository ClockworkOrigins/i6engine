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

#include "i6engine/rpg/npc/NPCManager.h"

#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/NPCParser.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ScriptingFacade.h"

namespace i6e {
namespace rpg {
namespace npc {

	NPCManager::NPCManager() : utils::Singleton<NPCManager>(), _npcs(), _running(true), _jobID() {
		_jobID = api::EngineController::GetSingleton().registerTimer(10000, std::bind(&NPCManager::checkNPCs, this), true, core::JobPriorities::Prio_Medium);
	}

	NPCManager::~NPCManager() {
		_running = false;
		api::EngineController::GetSingleton().removeTimerID(_jobID);
	}

	void NPCManager::createNPC(const std::string & identifier, const Vec3 & pos, const Quaternion & rot, bool player) {
		api::objects::GOTemplate tpl = NPCParser::GetSingletonPtr()->getTemplate(identifier);
		api::attributeMap paramsPSC;
		pos.insertInMap("pos", paramsPSC);
		rot.insertInMap("rot", paramsPSC);
		tpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

		if (!player) {
			api::attributeMap paramsDC;
			paramsDC.insert(std::make_pair("ident", identifier));
			tpl._components.push_back(api::objects::GOTemplateComponent("DialogChecker", paramsDC, "", false, false));
		} else {
			api::attributeMap paramsTPC;
			paramsTPC.insert(std::make_pair("ident", identifier));
			tpl._components.push_back(api::objects::GOTemplateComponent("ThirdPersonControl", paramsTPC, "", false, false));
		}

		NPC * n = new NPC(identifier, tpl, player);
		_npcs.insert(std::make_pair(identifier, n));
	}

	NPC * NPCManager::getNPC(const std::string & identifier) const {
		auto it = _npcs.find(identifier);
		if (it != _npcs.end()) {
			return it->second;
		}
		return nullptr;
	}

	bool NPCManager::checkNPCs() {
		for (auto & p : _npcs) {
			p.second->workQueue();
		}
		return _running;
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
