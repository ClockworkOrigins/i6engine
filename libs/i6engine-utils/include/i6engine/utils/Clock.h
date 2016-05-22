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
#include <vector>

#include "boost/bind.hpp"

namespace i6e {
namespace utils {

	template<class Updater>
	class Clock : public Updater {
	public:
		/**
		 * \brief default constructor
		 */
		Clock() : Updater(boost::bind(&Clock::Update, this)), _timer(), _lock(), _systemTime(0), _running(true) {
			Updater::Init();
		}

		/**
		 * \brief stops the updating of the clock, removes all timers
		 * \note make sure, this function is never called while an Update is still in progress
		 * It will call Updater::Stop befor the critical section begins.
		 */
		~Clock() {
			_running = false;
			Updater::Stop();
			for (size_t i = 0; i < _timer.size(); i++) {
				_timer[i].second->notify_all();
				delete _timer[i].second;
			}
			_timer.clear();
		}

		/**
		 * \brief updates current time and notifies timers, called from outside
		 * This function tells the clock to update it's current time.
		 * it will query the Updater template to get the current time value
		 * \note this function is not reentrant
		 */
		void Update() {
			// Get current time.
			_systemTime = Updater::getCurrentTime(_systemTime);
			notifyTimer();
		}

		/**
		 * \brief Will return the time since the Clock has been started
		 * \return Current time in microseconds
		 */
		inline uint64_t getTime() const {
			return _systemTime;
		}

		/**
		 * \brief registers a new timer waiting for this clock
		 */
		uint64_t registerTimer() {
			std::pair<uint64_t, std::condition_variable *> p(std::make_pair(UINT64_MAX, new std::condition_variable()));

			std::lock_guard<std::mutex> lock(_lock);
			_timer.push_back(p);

			return _timer.size() - 1;
		}

		/**
		 * \brief removes the given timer
		 * \note calling this while another function concerning this timer is pending
		 * (updateWaitTime etc.) can lead to undefined behaviour
		 */
		void unregisterTimer(uint64_t timerID) {
			std::lock_guard<std::mutex> lock(_lock);
			_timer[timerID].first = UINT64_MAX;
		}

		/**
		 * \brief updates the time a timer is waiting for
		 */
		void updateWaitTime(uint64_t timerID, uint64_t time) {
			if (time <= _systemTime) {
				std::lock_guard<std::mutex> lock(_lock);
				std::condition_variable * cond = _timer[size_t(timerID)].second;
				cond->notify_all();
				return;
			}
			std::lock_guard<std::mutex> lock(_lock);
			_timer[size_t(timerID)].first = time;
		}

		/**
		 * \brief let's a timer wait for the given time
		 */
		bool waitForTime(uint64_t timerID, uint64_t time) {
			if (time <= _systemTime) {
				return true;
			}
			std::unique_lock<std::mutex> lock(_lock);
			_timer[size_t(timerID)].first = time;
			_timer[size_t(timerID)].second->wait(lock);
			return _running;
		}

		/**
		 * \brief returns state of the Clock
		 */
		bool isRunning() const {
			return _running;
		}

	private:
		//        			wakeuptime      variable
		std::vector<std::pair<uint64_t, std::condition_variable *>> _timer;

		/**
		 * \brief lock to prevent conflicts regarding the timer vector
		 */
		mutable std::mutex _lock;

		// last system time
		std::atomic<uint64_t> _systemTime;

		std::atomic<bool> _running;

		/**
		 * \brief activates every timer waiting for current systemTime
		 */
		void notifyTimer() {
			std::unique_lock<std::mutex> lock(_lock);
			for (size_t i = 0; i < _timer.size(); i++) {
				if (_systemTime >= _timer[i].first) {
					_timer[i].first = UINT64_MAX;
					_timer[i].second->notify_all();
				}
			}
		}

		/**
		 * \brief forbidden
		 */
		Clock(const Clock &) = delete;
		Clock & operator=(const Clock &) = delete;
	};

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_CLOCK_H__ */

/**
 * @}
 */
