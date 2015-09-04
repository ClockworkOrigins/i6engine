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

#include "i6engine/core/clock/Timer.h"

#include "i6engine/core/EngineCoreController.h"

namespace i6engine {
namespace core {

	Timer::Timer(EngineCoreController * ctrl, const uint32_t lngFrameTime) : _ctrl(ctrl), _bolTimerStopped(false), _objCondExecutable(), _objCondMut(), _objCondUniqLock(_objCondMut), _timerID() {
		_timerID = _ctrl->registerTimer(lngFrameTime, boost::bind(&Timer::setLastUpdate, this), true, JobPriorities::Prio_Subsystem);
	}

	Timer::~Timer() {
		_bolTimerStopped = true;
		_objCondExecutable.wait(_objCondUniqLock);
	}

	void Timer::update() {
		if (!_bolTimerStopped) {
			// Pause the thread until it gets waked up by the Scheduler.
			_objCondExecutable.wait(_objCondUniqLock);
		}
	}

	bool Timer::setLastUpdate() {
		// Wake up the waiting thread.
		_objCondExecutable.notify_all();
		return !_bolTimerStopped;
	}

} /* namespace core */
} /* namespace i6engine */
