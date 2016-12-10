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

#include "i6engine/rpg/npc/NPC.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/ExperienceComponent.h"
#include "i6engine/rpg/components/InventoryComponent.h"
#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
#include "i6engine/rpg/dialog/DialogManager.h"
#include "i6engine/rpg/npc/NPCManager.h"
#include "i6engine/rpg/npc/queueJobs/SayJob.h"
#include "i6engine/rpg/npc/queueJobs/StartTradeJob.h"
#include "i6engine/rpg/npc/queueJobs/TurnToNpcJob.h"
#include "i6engine/rpg/npc/queueJobs/WaitSayJob.h"

namespace i6e {
namespace rpg {
namespace npc {

	NPC::NPC(const std::string & identifier, const api::objects::GOTemplate & tpl, bool player) : _go(), _queue(), _identifier(identifier) {
		if (!tpl._type.empty()) {
			i6eObjectFacade->createGO((player) ? "Player" : "NPC", tpl, i6eEngineController->getUUID(), false, [this](api::GOPtr go) {
				_go = go;
			});
		}
	}

	NPC::~NPC() {
	}

	std::string NPC::getIdentifier() const {
		return _identifier;
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

		auto playerList = i6eObjectFacade->getAllObjectsOfType("Player");
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

	uint32_t NPC::getCurrentXP() const {
		auto go = _go.get();
		if (go == nullptr) {
			return 0;
		}
		return go->getGOC<components::ExperienceComponent>(components::config::ComponentTypes::ExperienceComponent)->getCurrentXP();
	}

	void NPC::setCurrentXP(uint32_t xp) {
		auto go = _go.get();
		if (go == nullptr) {
			return;
		}
		return go->getGOC<components::ExperienceComponent>(components::config::ComponentTypes::ExperienceComponent)->setCurrentXP(xp);
	}

	uint32_t NPC::getNextXP() const {
		auto go = _go.get();
		if (go == nullptr) {
			return 0;
		}
		return go->getGOC<components::ExperienceComponent>(components::config::ComponentTypes::ExperienceComponent)->getNextXP();
	}

	void NPC::setNextXP(uint32_t xp) {
		auto go = _go.get();
		if (go == nullptr) {
			return;
		}
		return go->getGOC<components::ExperienceComponent>(components::config::ComponentTypes::ExperienceComponent)->setNextXP(xp);
	}

	uint32_t NPC::getLevel() const {
		auto go = _go.get();
		if (go == nullptr) {
			return 0;
		}
		return go->getGOC<components::ExperienceComponent>(components::config::ComponentTypes::ExperienceComponent)->getLevel();
	}

	void NPC::setLevel(uint32_t xp) {
		auto go = _go.get();
		if (go == nullptr) {
			return;
		}
		return go->getGOC<components::ExperienceComponent>(components::config::ComponentTypes::ExperienceComponent)->setLevel(xp);
	}

	void NPC::createItems(const std::string & identifier, uint32_t amount) {
		auto go = _go.get();
		if (go == nullptr) {
			return;
		}
		go->getGOC<components::InventoryComponent>(components::config::ComponentTypes::InventoryComponent)->createItems(identifier, amount);
	}

	void NPC::removeItems(const std::string & identifier, uint32_t amount) {
		auto go = _go.get();
		if (go == nullptr) {
			return;
		}
		go->getGOC<components::InventoryComponent>(components::config::ComponentTypes::InventoryComponent)->removeItems(identifier, amount);
	}

	void NPC::trade(NPC * other, double selfMultiplier, double otherMultiplier) {
		_queue.addJob(new StartTradeJob(this, other, selfMultiplier, otherMultiplier));
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
