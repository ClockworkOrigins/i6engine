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

#ifndef __I6ENGINE_CONFIGS_NETWORKCHANNELS_H__
#define __I6ENGINE_CONFIGS_NETWORKCHANNELS_H__

#ifdef ISIXE_NETWORK
	#include "m2etis/pubsub/config/ChannelName.h"

	// used for all messages containing information about GameObjects and their components
	#define OBJECT_CHANNEL m2etis::pubsub::I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP

	// used for alle position update messages
	#define POSITION_CHANNEL m2etis::pubsub::I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP

	// used only by server sending a request for ping to all clients
	#define PINGREQUEST_CHANNEL m2etis::pubsub::I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP

	// used only by clients to answer the servers ping request
	#define PINGANSWER_CHANNEL m2etis::pubsub::I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP

	// used for all chat messages
	#define CHAT_CHANNEL m2etis::pubsub::I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP
#else /* ISIXE_NETWORK */
	// used for all messages containing information about GameObjects and their components
	#define OBJECT_CHANNEL 0

	// used for alle position update messages
	#define POSITION_CHANNEL 0

	// used only by server sending a request for ping to all clients
	#define PINGREQUEST_CHANNEL 0

	// used only by clients to answer the servers ping request
	#define PINGANSWER_CHANNEL 0

	// used for all chat messages
	#define CHAT_CHANNEL 0
#endif /* ISIXE_NETWORK */

#endif /* __I6ENGINE_CONFIGS_NETWORKCHANNELS_H__ */
