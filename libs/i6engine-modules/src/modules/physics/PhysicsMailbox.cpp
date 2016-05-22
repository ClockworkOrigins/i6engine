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
