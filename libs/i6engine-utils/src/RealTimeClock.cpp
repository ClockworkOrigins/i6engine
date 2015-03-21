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

#include "i6engine/utils/RealTimeClock.h"

namespace i6engine {
namespace utils {

	RealTimeClock::RealTimeClock(const boost::function<void(void)> & f) : _startTime(boost::posix_time::microsec_clock::universal_time()), _offset(0), _update(f), _running(true), _thread(boost::bind(&RealTimeClock::clockUpdater, this)) {
	}

	RealTimeClock::~RealTimeClock() {
		Stop();
	}

	uint64_t RealTimeClock::getCurrentTime(uint64_t oldTime) const {
		return uint64_t(boost::posix_time::time_period(_startTime, boost::posix_time::microsec_clock::universal_time()).length().total_microseconds()) + _offset;
	}

	void RealTimeClock::setCurrentTime(uint64_t time) {
		_offset = time;
		_startTime = boost::posix_time::microsec_clock::universal_time();
	}

	void RealTimeClock::Stop() {
		_running = false;
		_thread.join();
	}

	void RealTimeClock::clockUpdater() {
		// need a bit of sleep until the clock is created completely
		// FIXME: (Michael) this is REALLY stupid
		boost::this_thread::sleep(boost::posix_time::microseconds(500));
		while (_running) {
			_update();
			boost::this_thread::sleep(boost::posix_time::microseconds(500));
		}
	}

} /* namespace utils */
} /* namespace i6engine */
