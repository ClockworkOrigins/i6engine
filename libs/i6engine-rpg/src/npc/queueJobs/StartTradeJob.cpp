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

#include "i6engine/rpg/npc/queueJobs/StartTradeJob.h"

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/InventoryComponent.h"

#include "i6engine/rpg/npc/NPC.h"

namespace i6e {
namespace rpg {
namespace npc {

	StartTradeJob::StartTradeJob(NPC * self, NPC * other, double selfMultiplier, double otherMultiplier) : _self(self), _other(other), _selfMultiplier(selfMultiplier), _otherMultiplier(otherMultiplier) {
	}

	void StartTradeJob::start() {
		_self->getGO()->getGOC<components::InventoryComponent>(components::config::ComponentTypes::InventoryComponent)->startTrade(_other->getGO()->getGOC<components::InventoryComponent>(components::config::ComponentTypes::InventoryComponent), _selfMultiplier, _otherMultiplier);
	}

	void StartTradeJob::loop() {
	}

	void StartTradeJob::finish() {
	}

	bool StartTradeJob::condition() {
		return true;
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
