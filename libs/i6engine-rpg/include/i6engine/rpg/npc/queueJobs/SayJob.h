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

#ifndef __I6ENGINE_RPG_NPC_QUEUE_SAYJOB_H__
#define __I6ENGINE_RPG_NPC_QUEUE_SAYJOB_H__

#include <cstdint>
#include <string>
#include <vector>

#include "i6engine/rpg/npc/NPCQueueJob.h"

namespace i6engine {
namespace rpg {
namespace npc {

	class NPC;
	class WaitSayJob;

	class SayJob : public NPCQueueJob {
	public:
		SayJob(NPC * self, const std::string & soundKey, const std::string & subtitleKey, const std::vector<WaitSayJob *> & jobs);

		~SayJob() {
		}

		void start();

		void loop();

		void finish();

		bool condition();

	private:
		NPC * _self;
		std::string _soundKey;
		std::string _subtitleKey;
		std::vector<WaitSayJob *> _jobs;
		uint64_t _startTime;
		uint64_t _subtitleDuration;
		bool _soundFound;
		bool _soundFinished;
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_NPC_QUEUE_SAYJOB_H__ */
