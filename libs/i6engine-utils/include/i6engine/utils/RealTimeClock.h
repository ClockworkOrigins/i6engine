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

#ifndef __I6ENGINE_UTILS_REALTIMECLOCK_H__
#define __I6ENGINE_UTILS_REALTIMECLOCK_H__

#include <atomic>
#include <chrono>

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/function.hpp"

namespace std {
	class thread;
} /* namespace std */

namespace i6e {
namespace utils {

	class ISIXE_UTILS_API RealTimeClock {
	public:
		/**
		 * \brief constructor taking the function to be called during update
		 */
		explicit RealTimeClock(const std::function<void(void)> & f);

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
		 * \brief initializes the clock
		 */
		void Init();

		/**
		 * \brief stops the clock
		 */
		void Stop();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;

		uint64_t _offset;

		std::function<void(void)> _update;

		std::atomic<bool> _running;

		std::thread * _thread;

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
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_REALTIMECLOCK_H__ */

/**
 * @}
 */
