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

#ifndef __I6ENGINE_CORE_ENGINECORECONTROLLER_H__
#define __I6ENGINE_CORE_ENGINECORECONTROLLER_H__

#include <condition_variable>
#include <mutex>
#include <vector>

#include "i6engine/utils/RealTimeClock.h"

#include "i6engine/core/clock/Scheduler.h"

#include "boost/function.hpp"

/**
 * \page page_engineCoreController EngineCoreController
 *
 * This EngineCoreController is the Object the Game communicates with most of the time.
 * After creation, it initialises the SubSystemController and than starts with its main loop.
 * Within this MainLoop, it just updates the Clock every 5 milliseconds.
 * After the MainLoop has finished, the EngineCoreController sends all SubSystems the shutdown command and waits for their termination. Than the whole Engine ends.
 * To be precise, the game developer creates an EngineHandle object that should be used for communicating, but currently it only offers the function modules::EngineModulesController& getEngineController(). This EngineModuleController is an abstraction layer for the EngineCoreController.\n
 * The whole initialising processlooks like this:
 * Game -> EngineHandle -> EngineModuleController -> EngineCoreController -> SubSystemController -> SubSystems
 */
namespace i6engine {
namespace core {

	class SubSystemController;

	/**
	 * \class EngineCoreController
	 * \brief this class is used as an abstraction layer
	 *
	 * The EngineCoreController initializes all registered subsystems and starts the main loop.
	 * The main loop gets the clock update and notifies the SubSystemController.
	 */
	class ISIXE_CORE_API EngineCoreController {
	public:
		/**
		 * \brief creates new EngineCoreController with given SubSystemController
		 */
		explicit EngineCoreController(SubSystemController * ssc);

		/**
		 * \brief This method will set the callback which will be called after initialization.
		 */
		void SetOnAfterInitializedCallback(const boost::function<void(void)> & ptrOnAfterInitialize) {
			_vptrOnAfterInitialize.push_back(ptrOnAfterInitialize);
		}

		/**
		 * \brief This method will initialize and start the game engine.
		 */
		void RunEngine();

		/**
		 * \brief This method will stop the main loop and will start the shut down sequence.
		 */
		void ShutDown() {
			_bolLoop = false;
			_condVar.notify_all();
		}

		/**
		 * \brief This method will tell the EngineController that the subsystems have been initialized.
		 */
		inline void SetInitialized() {
			_bolIsInitialized = true;
		}

		/**
		 * \brief This method will wait until all subsystems have been shut down.
		 */
		void WaitForShutDown() const;

		/**
		 * \brief This method initializes the final shutdown of the main loop.
		 */
		inline void SetShutdownComplete() {
			_bolShutdownComplete = true;
		}

		/**
		 * \brief registers a timer
		 * \param[in] name name of the timer to identify
		 * \param[in] time time in microseconds until func is called
		 * \param[in] func function to be called after given time
		 */
		uint64_t registerTimer(uint64_t time, const boost::function<bool(void)> & func, bool looping, JobPriorities priority);

		/**
		 * \brief deletes all timer with given priority
		 */
		void removeTimer(JobPriorities priority);

		/**
		 * \brief deletes all timer with given name
		 */
		bool removeTimerID(uint64_t id);

		/**
		 * \brief returns remaining time
		 */
		uint64_t getTimeLeft(uint64_t id) const;

		/**
		 * \brief returns current application time in microseconds
		 */
		uint64_t getCurrentTime() const {
			return _rClock.getCurrentTime(0);
		}

		/**
		 * \brief sets the current time (only at clients, time will be synchronized by server)
		 */
		void setCurrentTime(uint64_t time) {
			_rClock.setCurrentTime(time);
		}

	private:
		SubSystemController * _subsystemController;
		bool _bolIsInitialized;
		bool _bolLoop;
		bool _bolShutdownComplete;
		std::vector<boost::function<void(void)>> _vptrOnAfterInitialize;
		mutable std::mutex _lock;
		mutable std::condition_variable _condVar;

		utils::Clock<utils::RealTimeClock> _rClock;
		Scheduler<utils::RealTimeClock> _scheduler;

		/**
		 * \brief This method is the main loop for the engine.
		 */
		void MainLoop();

		/**
		 * \brief This method will initialize and start the game engine.
		 */
		void Run();

		/**
		 * \brief This method will initialize and start all subsystems.
		 */
		void Initialize() const;

		/**
		 * \brief forbidden
		 */
		EngineCoreController(const EngineCoreController &) = delete;

		/**
		 * \brief forbidden
		 */
		EngineCoreController & operator=(const EngineCoreController &) = delete;
	};

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_ENGINECORECONTROLLER_H__ */

/**
 * @}
 */
