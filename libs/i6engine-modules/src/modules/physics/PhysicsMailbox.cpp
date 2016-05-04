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

#include "i6engine/modules/physics/PhysicsMailbox.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/messaging/MessageStruct.h"

#include "i6engine/api/FrontendMessageTypes.h"

#include "i6engine/modules/physics/PhysicsManager.h"

namespace i6e {
namespace modules {

	void PhysicsMailbox::News(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getMessageType() == api::messages::PhysicsMessageType) {
			if (msg->getMethod() == core::Method::Create) {
				_manager->NewsCreate(msg);
			} else if (msg->getMethod() == core::Method::Update) {
				_manager->NewsUpdate(msg);
			} else if (msg->getMethod() == core::Method::Delete) {
				_manager->NewsDelete(msg);
			}
		} else if (msg->getMessageType() == api::messages::PhysicsNodeMessageType) {
			if (msg->getMethod() == core::Method::Create) {
				_manager->NewsCreate(msg);
			} else if (msg->getMethod() == core::Method::Update) {
				PhysicsNode * pn = _manager->getPhysicsNode(msg->getContent()->getWaitID());

				if (pn != nullptr) { // TODO: (Daniel) ensure this never happens in messaging subsystem!
					pn->News(msg);
				} else {
					ISIXE_LOG_ERROR("PhysicsMailbox", "PhysicsNode doesn't exist!");
				}
			} else if (msg->getMethod() == core::Method::Delete) {
				_manager->NewsDelete(msg);
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
