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

#ifndef __I6ENGINE_CORE_TIMER_H__
#define __I6ENGINE_CORE_TIMER_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include <condition_variable>
#include <mutex>

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_TIMER_H__ */

/**
 * @}
 */
