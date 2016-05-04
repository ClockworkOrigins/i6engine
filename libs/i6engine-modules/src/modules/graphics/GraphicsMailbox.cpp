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

#include "i6engine/modules/graphics/GraphicsMailbox.h"

#include "i6engine/api/FrontendMessageTypes.h"

#include "i6engine/modules/graphics/GraphicsManager.h"

#include "boost/shared_ptr.hpp"

namespace i6e {
namespace modules {

	GraphicsMailbox::GraphicsMailbox(GraphicsManager * manager) : _manager(manager) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	GraphicsMailbox::~GraphicsMailbox() {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	void GraphicsMailbox::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getMessageType() == api::messages::GraphicsMessageType) {
			if (msg->getMethod() == core::Method::Create) {
				_manager->NewsCreate(msg);
			} else if (msg->getMethod() == core::Method::Update) {
				_manager->NewsUpdate(msg);
			} else if (msg->getMethod() == core::Method::Delete) {
				_manager->NewsDelete(msg);
			}
		} else if (msg->getMessageType() == api::messages::GraphicsNodeMessageType) {
			if (msg->getMethod() == core::Method::Create) {
				_manager->NewsNodeCreate(msg);
			} else if (msg->getMethod() == core::Method::Update) {
				_manager->NewsNodeUpdate(msg);
			} else if (msg->getMethod() == core::Method::Delete) {
				_manager->NewsNodeDelete(msg);
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
