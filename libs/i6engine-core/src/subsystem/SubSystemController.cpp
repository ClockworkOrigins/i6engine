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

#include "i6engine/core/subsystem/SubSystemController.h"

#include "i6engine/core/EngineCoreController.h"
#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace core {

	SubSystemController::SubSystemController() : _bolGlobalShutDown(false), _bolWaitForInit(true), _objThreadGrp(), _objQueuedSubSystems(), _coreController(), _lock() {
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

	void SubSystemController::ShutDown() {
		_bolGlobalShutDown = true;

		for (QueuedModule & objQueuedSubSystem : _objQueuedSubSystems) {
			objQueuedSubSystem.module->stop();
		}
	}

	void SubSystemController::JoinAllSubsystems() {
		std::lock_guard<std::mutex> lg(_lock);
		for (boost::thread * t : _objThreadGrp) {
			t->join();
			delete t;
		}
		_objThreadGrp.clear();
		_objQueuedSubSystems.clear();
	}

	void SubSystemController::startSubSystemTicking(ModuleController * objSubSystem, const uint32_t lngFrameTime) {
		_objThreadGrp.push_back(new boost::thread(boost::bind(&ModuleController::startThreadTicking, objSubSystem, lngFrameTime)));

		while (_bolWaitForInit) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		}

		_bolWaitForInit = true;
	}

	void SubSystemController::startSubSystemWaiting(ModuleController * objSubSystem, const std::set<Subsystem> & waitingFor) {
		_objThreadGrp.push_back(new boost::thread(boost::bind(&ModuleController::startThreadWaiting, objSubSystem, waitingFor)));

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
		std::lock_guard<std::mutex> lg(_lock);
		for (QueuedModule & objQueuedSubSystem : _objQueuedSubSystems) {
			if (objQueuedSubSystem.module->getRunning()) {
				return false;
			}
		}
		ISIXE_LOG_INFO("SubSystemController", "all subsystems shut down");
		return true;
	}

} /* namespace core */
} /* namespace i6e */
