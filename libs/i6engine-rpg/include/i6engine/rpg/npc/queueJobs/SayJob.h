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

#ifndef __I6ENGINE_RPG_NPC_QUEUE_SAYJOB_H__
#define __I6ENGINE_RPG_NPC_QUEUE_SAYJOB_H__

#include <cstdint>
#include <string>
#include <vector>

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

#include "i6engine/rpg/npc/NPCQueueJob.h"

namespace i6e {
namespace rpg {
namespace npc {

	class NPC;
	class WaitSayJob;

	class SayJob : public NPCQueueJob, public api::MessageSubscriberFacade {
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
		uint64_t _soundHandle;

		void News(const api::GameMessage::Ptr & msg);
	};

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_NPC_QUEUE_SAYJOB_H__ */
