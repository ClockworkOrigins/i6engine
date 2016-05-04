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

#include "LuaMessagingExport.h"

#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace lua {
namespace messaging {

	void sendMessage(const i6e::api::GameMessage::Ptr & msg) {
		i6e::api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace messaging */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerMessaging() {
	return
		def("sendMessage", &i6e::lua::messaging::sendMessage)
		;
}
