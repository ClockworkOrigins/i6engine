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

#ifndef __I6ENGINE_UTILS_REALTIMECLOCK_H__
#define __I6ENGINE_UTILS_REALTIMECLOCK_H__

#include <atomic>

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"

namespace i6engine {
namespace utils {

	class ISIXE_UTILS_API RealTimeClock {
	public:
		/**
		 * \brief constructor taking the function to be called during update
		 */
		explicit RealTimeClock(const boost::function<void(void)> & f);

		/**
		 * \brief destructor, stopping threads and cleaning up
		 */
		~RealTimeClock();

		/**
		 * \brief returns the time between the current time and the creation of the clock in microseconds
		 */
		uint64_t getCurrentTime(uint64_t lastTime) const;

		/**
		 * \brief sets the current time (only at clients, time will be synchronized by server)
		 */
		void setCurrentTime(uint64_t time);

		/**
		 * \brief stops the clock
		 */
		void Stop();

	private:
		boost::posix_time::ptime _startTime;

		uint64_t _offset;

		boost::function<void(void)> _update;

		std::atomic<bool> _running;

		boost::thread _thread;

		/**
		 * \brief updater method
		 */
		void clockUpdater();

		/**
		 * \brief forbidden
		 */
		RealTimeClock & operator=(const RealTimeClock &) = delete;
		RealTimeClock(const RealTimeClock &) = delete;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_REALTIMECLOCK_H__ */

/**
 * @}
 */
