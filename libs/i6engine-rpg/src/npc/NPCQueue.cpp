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

#include "i6engine/rpg/npc/NPCQueue.h"

#include "i6engine/rpg/npc/NPCQueueJob.h"

namespace i6e {
namespace rpg {
namespace npc {

	NPCQueue::NPCQueue() : _queue() {
	}

	void NPCQueue::addJob(NPCQueueJob * job) {
		if (_queue.empty()) {
			job->start();
		}
		_queue.push(job);
	}

	void NPCQueue::checkJobs() {
		while (!_queue.empty()) {
			NPCQueueJob * job = _queue.front();
			if (!job->condition()) {
				job->loop();
				break;
			} else {
				job->finish();
				delete job;
				_queue.pop();
				if (!_queue.empty()) {
					_queue.front()->start();
				}
			}
		}
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
