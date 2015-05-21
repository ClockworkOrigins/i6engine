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

#ifndef __I6ENGINE_RPG_NPC_NPCQUEUE_H__
#define __I6ENGINE_RPG_NPC_NPCQUEUE_H__

#include "i6engine/utils/DoubleBufferQueue.h"

namespace i6engine {
namespace rpg {
namespace npc {

	class NPCQueueJob;

	class NPCQueue {
	public:
		NPCQueue();

		void addJob(NPCQueueJob * job);

		void checkJobs();

	private:
		utils::DoubleBufferQueue<NPCQueueJob *, true, false> _queue;

		NPCQueue(const NPCQueue &) = delete;
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_NPCQUEUE_H__ */