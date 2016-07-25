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

#include "i6engine/utils/RealTimeClock.h"

#include <iostream>
#include <thread>

namespace i6e {
namespace utils {

	RealTimeClock::RealTimeClock(const std::function<void(void)> & f) : _startTime(std::chrono::high_resolution_clock::now()), _offset(0), _update(f), _running(true), _thread(nullptr) {
	}

	RealTimeClock::~RealTimeClock() {
		Stop();
	}

	void RealTimeClock::Init() {
		_thread = new std::thread(std::bind(&RealTimeClock::clockUpdater, this));
	}

	uint64_t RealTimeClock::getCurrentTime(uint64_t) const {
		return uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _startTime).count()) + _offset;
	}

	void RealTimeClock::setCurrentTime(uint64_t time) {
		_offset = time;
		_startTime = std::chrono::high_resolution_clock::now();
	}

	void RealTimeClock::Stop() {
		if (_running) {
			_running = false;
			_thread->join();
			delete _thread;
		}
	}

	void RealTimeClock::clockUpdater() {
		while (_running) {
			_update();
			std::this_thread::sleep_for(std::chrono::microseconds(500));
		}
	}

} /* namespace utils */
} /* namespace i6e */
