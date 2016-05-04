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
