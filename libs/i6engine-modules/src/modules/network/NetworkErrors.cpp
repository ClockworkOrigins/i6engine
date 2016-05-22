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

#include "i6engine/modules/network/NetworkErrors.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/EngineController.h"

#include "i6engine/modules/network/NetworkManager.h"

namespace i6e {
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
} /* namespace i6e */
