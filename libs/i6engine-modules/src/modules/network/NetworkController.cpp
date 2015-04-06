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

#include "i6engine/modules/network/NetworkController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/network/NetworkMailbox.h"
#include "i6engine/modules/network/NetworkManager.h"

namespace i6engine {
namespace modules {

	NetworkController::NetworkController() : core::ModuleController(core::Subsystem::Network), _mailbox(), _manager() {
	}

	NetworkController::~NetworkController() {
	}

	void NetworkController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		ISIXE_LOG_INFO("NetworkController", "OnThreadStart: Starting");
		_mailbox = new NetworkMailbox();
		_manager = new NetworkManager(_mailbox);
		_mailbox->setManager(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::NetworkMessageType, NetworkMailbox::News, _mailbox);
		ISIXE_LOG_INFO("NetworkController", "OnThreadStart: Finished");
	}

	void NetworkController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::NetworkMessageType);
		ISIXE_LOG_INFO("NetworkController", "*** Shutting Down Subsystem start ***");
		delete _mailbox;
		delete _manager;

		ISIXE_LOG_INFO("NetworkController", "*** Shutting Down Subsystem end ***");
	}

	void NetworkController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

} /* namespace modules */
} /* namespace i6engine */
