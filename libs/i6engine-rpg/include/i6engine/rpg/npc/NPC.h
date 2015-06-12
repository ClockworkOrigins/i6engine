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

#ifndef __I6ENGINE_RPG_NPC_NPC_H__
#define __I6ENGINE_RPG_NPC_NPC_H__

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/weakPtr.h"

#include "i6engine/rpg/npc/NPCQueue.h"

namespace i6engine {
namespace api {
	class GameObject;
	typedef utils::sharedPtr<GameObject, GameObject> GOPtr;
	typedef utils::weakPtr<GameObject> WeakGOPtr;
namespace objects {
	struct GOTemplate;
} /* namespace objects */
} /* namespace api */
namespace rpg {
namespace npc {

	class ISIXE_RPG_API NPC {
	public:
		NPC(const api::objects::GOTemplate & tpl, bool player);
		NPC(const NPC & other) : _go(other._go), _queue() {
		}
		~NPC();

		api::GOPtr getGO() const {
			return _go.get();
		}

		void turnToNPC(NPC * npc);

		void say(const std::string & soundKey, const std::string & subtitleKey);

		uint32_t getItemCount(const std::string & identifier) const;

		uint32_t getCurrentXP() const;

		void setCurrentXP(uint32_t xp);

		uint32_t getNextXP() const;

		void setNextXP(uint32_t xp);

		uint32_t getLevel() const;

		void setLevel(uint32_t level);

		void addJob(NPCQueueJob * job) {
			_queue.addJob(job);
		}

		void workQueue() {
			_queue.checkJobs();
		}

		void createItems(const std::string & identifier, uint32_t amount);
		void removeItems(const std::string & identifier, uint32_t amount);

		void trade(NPC * other, double selfMultiplier, double otherMultiplier);

	private:
		api::WeakGOPtr _go;
		NPCQueue _queue;
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_NPC_H__ */
