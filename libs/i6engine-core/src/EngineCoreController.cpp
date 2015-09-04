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

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#define __STDC_LIMIT_MACROS
	#include <stdint.h>
#endif

#include "i6engine/core/EngineCoreController.h"

#include "i6engine/utils/ExceptionQueue.h"

#include "i6engine/core/subsystem/SubSystemController.h"

namespace i6engine {
namespace core {

	EngineCoreController::EngineCoreController(SubSystemController * ssc) : _subsystemController(ssc), _bolIsInitialized(false), _bolLoop(true), _bolShutdownComplete(false), _vptrOnAfterInitialize(), _lock(), _condVar(), _rClock(), _scheduler(_rClock) {
	}

	void EngineCoreController::Initialize() const {
		_subsystemController->Start();

		while (!_bolIsInitialized) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		}

		utils::exceptions::ExceptionQueue::GetSingleton().addCallback([this]() {
			_condVar.notify_all();
		});
	}

	void EngineCoreController::Run() {
		Initialize();

		for (size_t i = 0; i < _vptrOnAfterInitialize.size(); ++i) {
			if (_vptrOnAfterInitialize[i].vtable != nullptr) {
				_vptrOnAfterInitialize[i]();
			}
		}

		MainLoop();
		WaitForShutDown();
	}

	void EngineCoreController::RunEngine() {
		Run();
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
				boost::this_thread::sleep(boost::posix_time::milliseconds(5));

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

	uint64_t EngineCoreController::registerTimer(uint64_t time, const boost::function<bool(void)> & func, bool looping, JobPriorities priority /* = 0 */) {
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
} /* namespace i6engine */
