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

#include "i6engine/rpg/npc/NPCManager.h"

#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/NPCParser.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ScriptingFacade.h"

namespace i6engine {
namespace rpg {
namespace npc {

	NPCManager::NPCManager() : _npcs() {
	}

	NPCManager::~NPCManager() {
	}

	void NPCManager::createNPC(const std::string & identifier, const Vec3 & pos, bool player) {
		api::objects::GOTemplate tpl = NPCParser::GetSingletonPtr()->getTemplate(identifier);
		api::attributeMap paramsPSC;
		pos.insertInMap("pos", paramsPSC);
		tpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

		if (!player) {
			api::attributeMap paramsDC;
			paramsDC.insert(std::make_pair("ident", identifier));
			tpl._components.push_back(api::objects::GOTemplateComponent("DialogChecker", paramsDC, "", false, false));
		} else {
			api::attributeMap paramsTPC;
			paramsTPC.insert(std::make_pair("ident", identifier));
			tpl._components.push_back(api::objects::GOTemplateComponent("ThirdPersonControl", paramsTPC, "", false, false));
		}

		NPC * n = new NPC(tpl, player);
		_npcs.insert(std::make_pair(identifier, n));
#if ISIXE_SCRIPTING != SCRIPTING_NONE
		api::EngineController::GetSingletonPtr()->getScriptingFacade()->setGlobalVariable(identifier, n);
#endif
	}

	NPC * NPCManager::getNPC(const std::string & identifier) const {
		auto it = _npcs.find(identifier);
		if (it != _npcs.end()) {
			return it->second;
		}
		return nullptr;
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */
