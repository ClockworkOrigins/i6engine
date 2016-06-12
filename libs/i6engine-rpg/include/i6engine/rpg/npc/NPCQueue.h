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

#ifndef __I6ENGINE_RPG_NPC_NPCQUEUE_H__
#define __I6ENGINE_RPG_NPC_NPCQUEUE_H__

#include "clockUtils/container/DoubleBufferQueue.h"

namespace i6e {
namespace rpg {
namespace npc {

	class NPCQueueJob;

	/**
	 * \brief a queue every NPC has handling its jobs
	 */
	class NPCQueue {
	public:
		NPCQueue();

		/**
		 * \brief adds a job to the queue
		 */
		void addJob(NPCQueueJob * job);

		/**
		 * \brief handles current job, starts new job etc.
		 */
		void checkJobs();

	private:
		clockUtils::container::DoubleBufferQueue<NPCQueueJob *, true, false> _queue;

		NPCQueue(const NPCQueue &) = delete;
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_NPC_NPCQUEUE_H__ */

/**
 * @}
 */
