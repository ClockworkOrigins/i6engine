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

#include "i6engine/modules/gui/GUIMailbox.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "i6engine/modules/gui/GUIManager.h"

namespace i6engine {
namespace modules {

	GUIMailbox::GUIMailbox(GUIManager * manager) : _manager(manager) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	GUIMailbox::~GUIMailbox() {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	void GUIMailbox::News(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		uint16_t msgType = msg->getMessageType();

		uint16_t type = msg->getSubtype();

		core::Method method = msg->getMethod();

		if (msgType == api::messages::GUIMessageType) {
			api::gui::GUIUpdateMessageStruct * data = static_cast<api::gui::GUIUpdateMessageStruct *>(msg->getContent());
			if (method == core::Method::Create) {
				_manager->processCreateMessage(type, data);
			} else if (method == core::Method::Update) {
				_manager->processUpdateMessage(type, data);
			} else if (method == core::Method::Delete) {
				_manager->processDeleteMessage(type, data);
			}
		}

		if (msgType == api::messages::InputMessageType) {
			if (method == core::Method::Update) {
				_manager->processInputUpdateMessage(type, msg->getContent());
			}
		}
	}

} /* namespace modules */
} /* namespace i6engine */
