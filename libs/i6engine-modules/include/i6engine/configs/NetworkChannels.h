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

#ifndef __I6ENGINE_CONFIGS_NETWORKCHANNELS_H__
#define __I6ENGINE_CONFIGS_NETWORKCHANNELS_H__

#ifdef ISIXE_WITH_NETWORK
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
#else /* ISIXE_WITH_NETWORK */
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
#endif /* ISIXE_WITH_NETWORK */

#endif /* __I6ENGINE_CONFIGS_NETWORKCHANNELS_H__ */
