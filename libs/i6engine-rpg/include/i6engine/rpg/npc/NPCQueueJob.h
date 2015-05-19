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

#ifndef __I6ENGINE_RPG_NPC_NPCQUEUEJOB_H__
#define __I6ENGINE_RPG_NPC_NPCQUEUEJOB_H__

namespace i6engine {
namespace rpg {
namespace npc {

	class NPCQueueJob {
	public:
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
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_NPCQUEUEJOB_H__ */
