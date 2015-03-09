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

#include "i6engine/core/subsystem/SubSystemController.h"

#include "i6engine/core/EngineCoreController.h"
#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
namespace core {

	SubSystemController::SubSystemController() : _bolGlobalShutDown(false), _bolWaitForInit(true), _objThreadGrp(), _objQueuedSubSystems(), _coreController() {
	}

	void SubSystemController::Start() {
		for (QueuedModule & objQueuedSubSystem : _objQueuedSubSystems) {
			if (objQueuedSubSystem.type == SubsystemType::Ticking) {
				startSubSystemTicking(objQueuedSubSystem.module, objQueuedSubSystem.framerate);
			} else {
				startSubSystemWaiting(objQueuedSubSystem.module, objQueuedSubSystem.waitingFor);
			}
		}

		_coreController->SetInitialized();
	}

	void SubSystemController::startSubSystemTicking(ModuleController * objSubSystem, const uint32_t lngFrameTime) {
		_objThreadGrp.add_thread(new boost::thread(boost::bind(&ModuleController::startThreadTicking, objSubSystem, lngFrameTime)));

		while (_bolWaitForInit) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		}

		_bolWaitForInit = true;
	}

	void SubSystemController::startSubSystemWaiting(ModuleController * objSubSystem, const std::set<Subsystem> & waitingFor) {
		_objThreadGrp.add_thread(new boost::thread(boost::bind(&ModuleController::startThreadWaiting, objSubSystem, waitingFor)));

		while (_bolWaitForInit) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		}

		_bolWaitForInit = true;
	}

	void SubSystemController::QueueSubSystemStart(ModuleController * objSubsystem, const uint32_t lngFrameTime) {
		QueuedModule objQueuedSubSystem;
		objQueuedSubSystem.module = objSubsystem;
		objQueuedSubSystem.type = SubsystemType::Ticking;
		objQueuedSubSystem.framerate = lngFrameTime;
		_objQueuedSubSystems.push_back(objQueuedSubSystem);
	}

	void SubSystemController::QueueSubSystemStart(ModuleController * objSubsystem, const std::set<Subsystem> & waitingFor) {
		QueuedModule objQueuedSubSystem;
		objQueuedSubSystem.module = objSubsystem;
		objQueuedSubSystem.type = SubsystemType::Waiting;
		objQueuedSubSystem.waitingFor = waitingFor;
		_objQueuedSubSystems.push_back(objQueuedSubSystem);
	}

	bool SubSystemController::isShutdownComplete() {
		for (QueuedModule & objQueuedSubSystem : _objQueuedSubSystems) {
			if (objQueuedSubSystem.module->getRunning()) {
				return false;
			}
		}
		ISIXE_LOG_INFO("SubSystemController", "all subsystems shut down");

		return true;
	}

} /* namespace core */
} /* namespace i6engine */
