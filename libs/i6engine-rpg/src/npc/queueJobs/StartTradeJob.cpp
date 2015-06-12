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

#include "i6engine/rpg/npc/queueJobs/StartTradeJob.h"

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/InventoryComponent.h"

#include "i6engine/rpg/npc/NPC.h"

namespace i6engine {
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
} /* namespace i6engine */
