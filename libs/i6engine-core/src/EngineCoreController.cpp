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

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#define __STDC_LIMIT_MACROS
	#include <stdint.h>
#endif

#include "i6engine/core/EngineCoreController.h"

#include "i6engine/utils/ExceptionQueue.h"

#include "i6engine/core/subsystem/SubSystemController.h"

namespace i6e {
namespace core {

	EngineCoreController::EngineCoreController(SubSystemController * ssc) : _subsystemController(ssc), _bolIsInitialized(false), _bolLoop(true), _bolShutdownComplete(true), _vptrOnAfterInitialize(), _lock(), _condVar(), _runningLock(), _runningConditionVariable(), _rClock(), _scheduler(_rClock) {
	}

	EngineCoreController::~EngineCoreController() {
		std::unique_lock<std::mutex> ul(_runningLock);
		if (!_bolShutdownComplete) {
			_runningConditionVariable.wait(ul);
		}
	}

	void EngineCoreController::Initialize() const {
		_subsystemController->Start();

		while (!_bolIsInitialized) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		utils::exceptions::ExceptionQueue::GetSingleton().addCallback([this]() {
			std::unique_lock<std::mutex> ul(_lock);
			_condVar.notify_all();
		});
	}

	void EngineCoreController::Run() {
		Initialize();

		for (size_t i = 0; i < _vptrOnAfterInitialize.size(); ++i) {
			if (_vptrOnAfterInitialize[i] != nullptr) {
				_vptrOnAfterInitialize[i]();
			}
		}

		MainLoop();
		WaitForShutDown();
	}

	void EngineCoreController::RunEngine() {
		std::unique_lock<std::mutex> ul(_runningLock);
		_bolShutdownComplete = false;
		Run();
		_runningConditionVariable.notify_one();
	}

	void EngineCoreController::MainLoop() {
		while (_bolLoop) {
			try {
				if (!utils::exceptions::ExceptionQueue::GetSingleton().isEmpty()) {
					utils::exceptions::loginfo info = utils::exceptions::ExceptionQueue::GetSingleton().dequeue();
					throw utils::exceptions::SystemFailureException(info.module, info.message, info.file, info.line);
				}
			} catch (utils::exceptions::SystemFailureException & e) {
				// repair if possible
				e.writeLog();
				exit(1);
			} catch (utils::exceptions::i6exception & e) {
				e.writeLog();
				exit(1);
			}
			std::unique_lock<std::mutex> ul(_lock);
			_condVar.wait(ul);
		}

		_subsystemController->ShutDown();

		while (!_bolShutdownComplete) {
			try {
				if (_subsystemController->isShutdownComplete()) {
					_bolShutdownComplete = true;
				}

				// throttle main thread
				std::this_thread::sleep_for(std::chrono::milliseconds(5));

				if (!utils::exceptions::ExceptionQueue::GetSingleton().isEmpty()) {
					utils::exceptions::loginfo info = utils::exceptions::ExceptionQueue::GetSingleton().dequeue();
					throw utils::exceptions::SystemFailureException(info.module, info.message, info.file, info.line);
				}
			} catch (utils::exceptions::SystemFailureException & e) {
				// repair if possible
				e.writeLog();
				exit(1);
			} catch (utils::exceptions::i6exception & e) {
				e.writeLog();
				exit(1);
			}
		}
	}

	void EngineCoreController::WaitForShutDown() const {
		_subsystemController->JoinAllSubsystems();
	}

	uint64_t EngineCoreController::registerTimer(uint64_t time, const std::function<bool(void)> & func, bool looping, JobPriorities priority /* = 0 */) {
		if (looping) {
			return _scheduler.runRepeated(time, func, priority);
		} else {
			return _scheduler.runOnce(time, func, priority);
		}
	}

	void EngineCoreController::removeTimer(JobPriorities priority) {
		_scheduler.removeTimer(priority);
	}

	bool EngineCoreController::removeTimerID(uint64_t id) {
		return _scheduler.stop(id);
	}

	uint64_t EngineCoreController::getTimeLeft(uint64_t id) const {
		return _scheduler.getTimeLeft(id);
	}

} /* namespace core */
} /* namespace i6e */
