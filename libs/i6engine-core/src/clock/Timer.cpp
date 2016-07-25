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

#include "i6engine/core/clock/Timer.h"

#include "i6engine/core/EngineCoreController.h"

namespace i6e {
namespace core {

	Timer::Timer(EngineCoreController * ctrl, const uint32_t lngFrameTime) : _ctrl(ctrl), _bolTimerStopped(false), _condVariable(), _lock(), _timerID() {
		_timerID = _ctrl->registerTimer(lngFrameTime, std::bind(&Timer::setLastUpdate, this), true, JobPriorities::Prio_Subsystem);
	}

	Timer::~Timer() {
		_bolTimerStopped = true;
		std::unique_lock<std::mutex> ul(_lock);
		_condVariable.wait(ul);
	}

	void Timer::update() {
		if (!_bolTimerStopped) {
			// Pause the thread until it gets waked up by the Scheduler.
			std::unique_lock<std::mutex> ul(_lock);
			_condVariable.wait(ul);
		}
	}

	bool Timer::setLastUpdate() {
		// Wake up the waiting thread.
		std::unique_lock<std::mutex> ul(_lock);
		_condVariable.notify_all();
		return !_bolTimerStopped;
	}

} /* namespace core */
} /* namespace i6e */
