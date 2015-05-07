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

#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/core/messaging/MessageSubscriber.h"
#include "i6engine/core/messaging/MessagingController.h"

namespace i6engine {
namespace api {

	MessagingFacade::MessagingFacade() : _messagingController(nullptr) {
	}

	MessagingFacade::~MessagingFacade() {}

	void MessagingFacade::deliverMessage(const GameMessage::Ptr & msg) {
		_messagingController->deliverMessage(msg);
	}

	void MessagingFacade::registerMessageType(uint16_t msgType, core::MessageSubscriber * objMessageSubscriber, const boost::function<void(GameMessage::Ptr)> & ptrMessageMethod) {
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
} /* namespace i6engine */
