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

#include "i6engine/modules/network/NetworkController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/network/NetworkMailbox.h"
#include "i6engine/modules/network/NetworkManager.h"

namespace i6e {
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
} /* namespace i6e */
