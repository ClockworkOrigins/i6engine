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

#include "i6engine/modules/pythonscripting/PythonScriptingMailbox.h"

#include "i6engine/api/FrontendMessageTypes.h"

#include "i6engine/modules/pythonscripting/PythonScriptingManager.h"

namespace i6e {
namespace modules {

	PythonScriptingMailbox::PythonScriptingMailbox(PythonScriptingManager * m) : _manager(m) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	void PythonScriptingMailbox::News(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t msgType = msg->getMessageType();

		if (msgType == api::messages::ScriptingMessageType) {
			_manager->News(msg);
		}
	}

} /* namespace modules */
} /* namespace i6e */
