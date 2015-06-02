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

#include "i6engine/rpg/npc/NPC.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/InventoryComponent.h"
#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
#include "i6engine/rpg/dialog/DialogManager.h"
#include "i6engine/rpg/npc/NPCManager.h"
#include "i6engine/rpg/npc/queueJobs/SayJob.h"
#include "i6engine/rpg/npc/queueJobs/TurnToNpcJob.h"
#include "i6engine/rpg/npc/queueJobs/WaitSayJob.h"

namespace i6engine {
namespace rpg {
namespace npc {

	NPC::NPC(const api::objects::GOTemplate & tpl, bool player) : _go(), _queue() {
		api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO((player) ? "Player" : "NPC", tpl, api::EngineController::GetSingletonPtr()->getUUID(), false, [this](api::GOPtr go) {
			_go = go;
		});
	}

	NPC::~NPC() {
	}

	void NPC::turnToNPC(NPC * npc) {
		_queue.addJob(new TurnToNpcJob(_go, npc->_go));
	}

	void NPC::say(const std::string & soundKey, const std::string & subtitleKey) {
		std::vector<std::string> npcs = dialog::DialogManager::GetSingleton().getActiveNPCs();

		std::vector<NPC *> realNPCs;

		for (std::string s : npcs) {
			NPC * n = NPCManager::GetSingleton().getNPC(s);
			if (n != this) {
				realNPCs.push_back(n);
			}
		}

		auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
		auto player = *playerList.begin();
		npc::NPC * p = npc::NPCManager::GetSingleton().getNPC(player->getGOC<components::ThirdPersonControlComponent>(components::config::ComponentTypes::ThirdPersonControlComponent)->getNPCIdentifier());

		if (this != p) {
			realNPCs.push_back(p);
		}

		std::vector<WaitSayJob *> waitJobs;

		for (NPC * n : realNPCs) {
			WaitSayJob * job = new WaitSayJob();
			n->addJob(job);
			waitJobs.push_back(job);
		}

		_queue.addJob(new SayJob(this, soundKey, subtitleKey, waitJobs));
	}

	uint32_t NPC::getItemCount(const std::string & identifier) const {
		auto go = _go.get();
		if (go == nullptr) {
			return 0;
		}
		return go->getGOC<components::InventoryComponent>(components::config::ComponentTypes::InventoryComponent)->getItemCount(identifier);
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */
