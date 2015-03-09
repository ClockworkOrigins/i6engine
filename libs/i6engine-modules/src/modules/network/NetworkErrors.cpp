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

#include "i6engine/modules/network/NetworkErrors.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/EngineController.h"

#include "i6engine/modules/network/NetworkManager.h"

namespace i6engine {
namespace modules {

	NetworkErrors::NetworkErrors(NetworkManager * manager) : _manager(manager) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	NetworkErrors::~NetworkErrors() {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	void NetworkErrors::connectionFailed(const std::string & other) {
		_manager->connectionFailed();

		api::EngineController::GetSingletonPtr()->getAppl()->connectionFailed(core::IPKey(other));
	}

	void NetworkErrors::connectionClosed(const std::string & other) {
		api::EngineController::GetSingletonPtr()->getAppl()->disconnectCallback(core::IPKey(other));
	}

	void NetworkErrors::bindFailure(const std::string & other) {
		_manager->connectionFailed();

		api::EngineController::GetSingletonPtr()->getAppl()->bindFailure();
	}

} /* namespace modules */
} /* namespace i6engine */
