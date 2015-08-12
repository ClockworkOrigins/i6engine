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
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_SCHEDULER_H__
#define __I6ENGINE_CORE_SCHEDULER_H__

#include <queue>

#include "i6engine/utils/Clock.h"
#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/JobPriorities.h"
#include "i6engine/core/configs/SchedulerConfig.h"

#include "boost/thread.hpp"

namespace i6engine {
namespace core {

	template<class ClockUpdater>
	class Scheduler {
	public:
		/**
		 * \brief this struct represents a single timer event
		 */
		typedef struct Job {
			/**
			 * \brief constructor of the timer
			 */
			Job(const boost::function<bool(void)> & f, uint64_t t, JobPriorities p, uint64_t d, uint64_t i = UINT64_MAX) : func(f), time(t), priority(p), interval(i), id(d) {
			}

			/**
			 * \brief method called by this timer
			 */
			boost::function<bool(void)> func;

			/**
			 * \brief time this job will be scheduled
			 */
			uint64_t time;

			/**
			 * \brief priority of this job
			 */
			JobPriorities priority; // the lower, the better

			/**
			 * \brief interval in which this job is repeated, LONG_MAX if only once
			 */
			uint64_t interval;

			/**
			 * \brief id of this job
			 */
			uint64_t id;

			/**
			 * \brief operator for lower than, needed for priority_queue
			 */
			bool operator<(const Job & other) const {
				if (time != other.time) {
					return time > other.time;
				}
				if (priority != other.priority) {
					return priority > other.priority;
				}
				return interval > other.interval;
			}
		} Job;

		/**
		 * \brief constructor for scheduler taking a clock
		 */
		explicit Scheduler(utils::Clock<ClockUpdater> & c) : _running(true), _clock(c), _queue(), _lock(), _tID(_clock.registerTimer()), _worker(boost::bind(&Scheduler<ClockUpdater>::worker, this)), _id() {
		}

		/**
		 * \brief destructor
		 */
		~Scheduler() {
			_running = false;
			_lock.lock();
			while (!_queue.empty()) {
				_queue.pop();
			}
			_lock.unlock();
			_worker.interrupt();
			_clock.updateWaitTime(_tID, 0);
			_worker.join();
		}

		/**
		 * \brief starts a timer beeing scheduled after the given time
		 *
		 * \param[in] name name of the timer
		 * \param[in] time the time until calling the given method
		 * \param[in] f the method to be called if time is over
		 * \param[in] priority the priority in which order the timers are scheduled if time is equal
		 */
		uint64_t runOnce(uint64_t time, const boost::function<bool(void)> & f, JobPriorities priority) {
			if (time <= 0) {
				ISIXE_THROW_API("Scheduler", "time need to be > 0");
			}

			Job j(f, _clock.getTime() + time, priority, _id++);
			boost::mutex::scoped_lock sl(_lock);
			_queue.push(j);
			if (_queue.top().time == j.time) {
				_clock.updateWaitTime(_tID, j.time);
			}

			return j.id;
		}

		/**
		 * \brief starts a timer repeating in the given interval
		 *
		 * \param[in] name name of the timer
		 * \param[in] interval the time between repetition of this method
		 * \param[in] f the method to be called if time is over
		 * \param[in] priority the priority in which order the timers are scheduled if time is equal
		 */
		uint64_t runRepeated(uint64_t interval, const boost::function<bool(void)> & f, JobPriorities priority) {
			if (interval <= 0) {
				ISIXE_THROW_API("Scheduler", "interval has to be greater than 0, otherwise there would be an instant call");
			}

			Job j(f, _clock.getTime() + interval, priority, _id++, interval);
			boost::mutex::scoped_lock sl(_lock);
			_queue.push(j);
			if (_queue.top().time == j.time) {
				_clock.updateWaitTime(_tID, j.time);
			}
			return j.id;
		}

		/**
		 * \brief returns time left until execution of the job
		 * if no job with given name found, -1 is returned
		 */
		uint64_t getTimeLeft(uint64_t id) const {
			_lock.lock();
			std::priority_queue<Job> copy = _queue;
			_lock.unlock();

			while (!copy.empty()) {
				Job j = copy.top();
				copy.pop();

				if (j.id == id) {
					return j.time - _clock.getTime();
				}
			}

			return UINT64_MAX;
		}

		/**
		 * \brief removes all timers with given priority
		 */
		void removeTimer(JobPriorities priority) {
			boost::mutex::scoped_lock sl(_lock);
			std::priority_queue<Job> copy = _queue;

			while (!_queue.empty()) {
				_queue.pop();
			}

			while (!copy.empty()) {
				Job j = copy.top();
				copy.pop();

				if (j.priority != priority) {
					_queue.push(j);
				}
			}
		}

		/**
		 * \brief removes Timer registered for given id
		 * returns true, if Timer was found, otherwise false
		 */
		bool stop(uint64_t id) {
			boost::mutex::scoped_lock sl(_lock);
			std::priority_queue<Job> copy = _queue;

			while (!_queue.empty()) {
				_queue.pop();
			}

			bool b = false;

			while (!copy.empty()) {
				Job j = copy.top();
				copy.pop();

				if (j.id != id) {
					_queue.push(j);
				} else {
					b = true;
				}
			}

			return b;
		}

	private:
		// TODO: (Michael) convert to a threadpool if needed
		/**
		 * \brief waits until a job is done and handles this stuff
		 */
		void worker() {
			while (_running) {
				_lock.lock();
				while (!_queue.empty() && _queue.top().time <= _clock.getTime()) {
					Job j = _queue.top();
					_queue.pop();
					_lock.unlock();
					bool b = j.func();
					_lock.lock();
					if (j.interval != UINT64_MAX && b) {
						j.time = _clock.getTime() + j.interval;
						_queue.push(j);
					}
				}
				uint64_t t = _clock.getTime() + 1000000; // sleep 1 second if no task is there
				if (!_queue.empty()) {
					t = _queue.top().time;
				}
				_lock.unlock();
				if (!_clock.waitForTime(_tID, t)) {
					break;
				}
			}
		}

		/**
		 * \brief says wether this thread is running or not
		 */
		std::atomic<bool> _running;

		/**
		 * \brief the internal clock being used for time management
		 */
		utils::Clock<ClockUpdater> & _clock;

		/**
		 * \brief queue containing all current jobs
		 */
		std::priority_queue<Job> _queue;

		/**
		 * \brief mutex for thread safety
		 */
		mutable boost::mutex _lock;

		/**
		 * \brief tID of the timer used
		 */
		uint64_t _tID;

		/**
		 * \brief internal thread for worker method
		 */
		boost::thread _worker;

		std::atomic<uint64_t> _id;

		/**
		 * \brief forbidden
		 */
		Scheduler(const Scheduler &) = delete;
		Scheduler & operator=(const Scheduler &) = delete;
	};

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_SCHEDULER_H__ */

/**
 * @}
 */
