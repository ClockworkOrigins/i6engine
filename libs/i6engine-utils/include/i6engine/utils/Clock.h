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

/**
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_CLOCK_H__
#define __I6ENGINE_UTILS_CLOCK_H__

#include <atomic>
#include <cassert>
#include <climits>
#include <condition_variable>
#include <map>

#include "boost/bind.hpp"

namespace i6engine {
namespace utils {

	template<class Updater>
	class Clock : public Updater {
	public:
		/**
		 * \brief default constructor
		 */
		Clock() : Updater(boost::bind(&Clock::Update, this)), _timer(), _lock(), _usedIDs(0), _systemTime(0), _running(true) {
		}

		/**
		 * \brief stops the updating of the clock, removes all timers
		 */
		~Clock() {
			_running = false;
			Updater::Stop();
			for (std::map<uint64_t, std::pair<uint64_t, std::condition_variable *>>::iterator it = _timer.begin(); it != _timer.end(); ++it) {
				it->second.second->notify_all();
				delete it->second.second;
			}
			_timer.clear();
		}

		/**
		 * \brief Will return the time since the Clock has been started
		 * \return Current time in microseconds
		 */
		inline uint64_t getTime() const {
			return _systemTime;
		}

		/**
		 * \brief updates current time and notifies timers, called from outside
		 */
		void Update() {
			if (!_running) {
				return;
			}
			// Get current time.
			std::lock_guard<std::mutex> sl(_lock);
			_systemTime = Updater::getCurrentTime(_systemTime);
			notifyTimer();
		}

		/**
		 * \brief registers a new timer waiting for this clock
		 */
		uint64_t registerTimer() {
			std::pair<uint64_t, std::condition_variable *> p(std::make_pair(UINT64_MAX, new std::condition_variable()));

			uint64_t id = _usedIDs++;
			std::lock_guard<std::mutex> lock(_lock);
			_timer.insert(std::make_pair(id, p));

			return id;
		}

		/**
		 * \brief removes the given timer
		 */
		void unregisterTimer(uint64_t timerID) {
			std::lock_guard<std::mutex> lock(_lock);
			delete _timer[timerID].second;
			_timer.erase(timerID);
		}

		/**
		 * \brief updates the time a timer is waiting for
		 */
		void updateWaitTime(uint64_t timerID, uint64_t time) {
			std::lock_guard<std::mutex> lock(_lock);
			if (time <= _systemTime) {
				_timer[timerID].second->notify_all();
				return;
			}
			_timer[timerID].first = time;
		}

		/**
		 * \brief lets a time wait for the given time
		 */
		bool waitForTime(uint64_t timerID, uint64_t time) {
			if (!_running) {
				return false;
			}
			if (time <= _systemTime) {
				return true;
			}
			std::unique_lock<std::mutex> lock(_lock);
			auto it = _timer.find(timerID);
			it->second.first = time;
			it->second.second->wait(lock);
			return _running;
		}

		/**
		 * \brief returns state of the Clock
		 */
		bool isRunning() const {
			return _running;
		}

	private:
		/**
		 * \brief forbidden
		 */
		Clock(const Clock &) = delete;
		Clock & operator=(const Clock &) = delete;

		/**
		 * \brief activates every timer waiting for current systemTime
		 */
		void notifyTimer() {
			for (std::map<uint64_t, std::pair<uint64_t, std::condition_variable *>>::iterator it = _timer.begin(); it != _timer.end(); ++it) {
				if (_systemTime >= it->second.first) {
					it->second.first = UINT64_MAX;
					it->second.second->notify_all();
				}
			}
		}

		//        id            wakeuptime      variable
		std::map<uint64_t, std::pair<uint64_t, std::condition_variable *>> _timer;

		mutable std::mutex _lock;
		std::atomic<uint64_t> _usedIDs;

		// last system time
		std::atomic<uint64_t> _systemTime;

		std::atomic<bool> _running;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_CLOCK_H__ */

/**
 * @}
 */
