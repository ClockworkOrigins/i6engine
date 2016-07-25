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

#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/core/messaging/MessageSubscriber.h"
#include "i6engine/core/messaging/MessagingController.h"

namespace i6e {
namespace api {

	MessagingFacade::MessagingFacade() : _messagingController(nullptr) {
	}

	MessagingFacade::~MessagingFacade() {}

	void MessagingFacade::deliverMessage(const GameMessage::Ptr & msg) {
		_messagingController->deliverMessage(msg);
	}

	void MessagingFacade::registerMessageType(uint16_t msgType, core::MessageSubscriber * objMessageSubscriber, const std::function<void(GameMessage::Ptr)> & ptrMessageMethod) {
		objMessageSubscriber->addMethod(msgType, ptrMessageMethod);

		_messagingController->registerMessageType(msgType, objMessageSubscriber);
	}

	void MessagingFacade::unregisterMessageType(uint16_t msgType, core::MessageSubscriber * objMessageSubscriber) {
		objMessageSubscriber->removeMethod(msgType);

		_messagingController->unregisterMessageType(msgType, objMessageSubscriber);
	}

	void MessagingFacade::registerMessagingController(core::MessagingController * ctrl) {
		_messagingController = ctrl;
	}

} /* namespace api */
} /* namespace i6e */
