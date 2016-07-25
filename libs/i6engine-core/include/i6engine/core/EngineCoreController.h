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
namespace i6e {
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

		~EngineCoreController();

		/**
		 * \brief This method will set the callback which will be called after initialization.
		 */
		void SetOnAfterInitializedCallback(const std::function<void(void)> & ptrOnAfterInitialize) {
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
			std::unique_lock<std::mutex> ul(_lock);
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
		uint64_t registerTimer(uint64_t time, const std::function<bool(void)> & func, bool looping, JobPriorities priority);

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
		std::vector<std::function<void(void)>> _vptrOnAfterInitialize;
		mutable std::mutex _lock;
		mutable std::condition_variable _condVar;
		std::mutex _runningLock;
		std::condition_variable _runningConditionVariable;

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
} /* namespace i6e */

#endif /* __I6ENGINE_CORE_ENGINECORECONTROLLER_H__ */

/**
 * @}
 */
