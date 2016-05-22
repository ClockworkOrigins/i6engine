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

#ifndef __I6ENGINE_RPG_NPC_NPCQUEUEJOB_H__
#define __I6ENGINE_RPG_NPC_NPCQUEUEJOB_H__

namespace i6e {
namespace rpg {
namespace npc {

	/**
	 * \brief abstract base class for all jobs for NPCQueue
	 */
	class NPCQueueJob {
	public:
		virtual ~NPCQueueJob() {
		}

		/**
		 * \brief called when job is created
		 * use this method for initialization
		 */
		virtual void start() = 0;

		/**
		 * \brief called during every check
		 * use for stuff to be done more often
		 */
		virtual void loop() = 0;

		/**
		 * \brief called before job is deleted
		 * use this method for cleanup
		 */
		virtual void finish() = 0;

		/**
		 * \brief returns true if job is completed and can be removed
		 */
		virtual bool condition() = 0;
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_NPC_NPCQUEUEJOB_H__ */

/**
 * @}
 */
