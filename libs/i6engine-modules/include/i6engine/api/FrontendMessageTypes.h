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

/**
 * \addtogroup Messaging
 * @{
 */

#ifndef __I6ENGINE_API_FRONTENDMESSAGETYPES_H__
#define __I6ENGINE_API_FRONTENDMESSAGETYPES_H__

#include "i6engine/core/messaging/MessageTypes.h"

namespace i6engine {
namespace api {
namespace messages {

	enum EngineMessageTypes {
		AdministrationMessageType = core::messages::MessageTypesCount,
		ComponentMessageType,
		GameMessageType,
		GraphicsMessageType,
		GraphicsNodeMessageType,
		GUIMessageType,
		InputMessageType,
		ObjectManagerMessageType,
		NetworkMessageType,
		ObjectMessageType,
		PhysicsMessageType,
		PhysicsNodeMessageType,
		ScriptingMessageType,
		PositionMessageType,
		AudioMessageType,
		AudioNodeMessageType,
		MessageTypeCount
	};

} /* namespace messages */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_FRONTENDMESSAGETYPES_H__ */

/**
 * @}
 */
