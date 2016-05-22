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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_NPC_NPC_H__
#define __I6ENGINE_RPG_NPC_NPC_H__

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/weakPtr.h"

#include "i6engine/rpg/npc/NPCQueue.h"

namespace i6e {
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

	/**
	 * \brief represents one NPC
	 */
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
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_NPC_NPC_H__ */

/**
 * @}
 */
