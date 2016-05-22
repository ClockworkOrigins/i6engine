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

#include "i6engine/modules/gui/GUIMailbox.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "i6engine/modules/gui/GUIManager.h"

namespace i6e {
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
} /* namespace i6e */
