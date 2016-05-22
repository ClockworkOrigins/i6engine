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

namespace i6e {
namespace utils {

	RealTimeClock::RealTimeClock(const boost::function<void(void)> & f) : _startTime(std::chrono::high_resolution_clock::now()), _offset(0), _update(f), _running(true), _thread() {
	}

	RealTimeClock::~RealTimeClock() {
		Stop();
	}

	void RealTimeClock::Init() {
		_thread = boost::thread(boost::bind(&RealTimeClock::clockUpdater, this));
	}

	uint64_t RealTimeClock::getCurrentTime(uint64_t) const {
		return uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _startTime).count()) + _offset;
	}

	void RealTimeClock::setCurrentTime(uint64_t time) {
		_offset = time;
		_startTime = std::chrono::high_resolution_clock::now();
	}

	void RealTimeClock::Stop() {
		_running = false;
		_thread.join();
	}

	void RealTimeClock::clockUpdater() {
		while (_running) {
			_update();
			boost::this_thread::sleep(boost::posix_time::microseconds(500));
		}
	}

} /* namespace utils */
} /* namespace i6e */
