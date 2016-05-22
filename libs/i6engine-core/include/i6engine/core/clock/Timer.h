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
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_TIMER_H__
#define __I6ENGINE_CORE_TIMER_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include <condition_variable>
#include <mutex>

namespace i6e {
namespace core {

	class EngineCoreController;

	/**
	 * \class Timer
	 * \brief This class provides gametime-synchronized Timers. They are controlled by the Scheduler.
	 *
	 * A timer is always immortal until its deletion.
	 * When the frame time has been expired, the timer gets an update from the Scheduler. Eventual threads waiting in the Update method get awoken
	 * by the main thread.
	 * The immortal timer will shut down when the subsystem shuts down.
	 *
	 * \internal To get an overview of the Clock system in German, have a look at our wiki (topic: 'Taktgebung in der i6engine' )
	 */
	class Timer {
	public:
		/**
		 * \brief Constructor for an immortal timer
		 * \param lngFrametime This interval given in microseconds determines the update frequency e.g. the time between two calls of ptrFunc.
		 */
		Timer(EngineCoreController * ctrl, const uint32_t lngFrametime);

		/**
		 * \brief destructor removes timer
		 */
		~Timer();

		/**
		 * \brief Start the timer so an update can be performed later.
		 * If the timer has not been started, the update method will do nothing.
		 */
		inline void start() { _bolTimerStopped = false; }

		/**
		 * \brief Pause the timer so an update will do nothing.
		 */
		inline void pause() { _bolTimerStopped = true; }

		/**
		 * \brief This method executes the given update function ptrFun() and lets the thread wait at a conditional variable.
		 */
		void update();

		/**
		 * \brief registered as job in Scheduler to be called when time is over
		 */
		bool setLastUpdate();

	private:
		/**
		 * \brief forbidden
		 */
		Timer() = delete;

		/**
		 * \brief forbidden
		 */
		Timer(const Timer &) = delete;

		/**
		 * \brief forbidden
		 */
		Timer & operator=(const Timer &) = delete;

		EngineCoreController * _ctrl;

		bool _bolTimerStopped;
		std::condition_variable _condVariable;
		std::mutex _lock;
		uint64_t _timerID;
	};

} /* namespace core */
} /* namespace i6e */

#endif /* __I6ENGINE_CORE_TIMER_H__ */

/**
 * @}
 */
