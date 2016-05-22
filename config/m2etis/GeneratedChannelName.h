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

#ifndef m2etis_GeneratedChannelNameExample_h
#define m2etis_GeneratedChannelNameExample_h

namespace m2etis {
    namespace pubsub {

        const static uint16_t CHANNEL_COUNT = 1;
#define M2ETIS_CHANNELLIST I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP
		enum ChannelName : uint16_t {
        	I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP	// Position, Ping, PingAnswer, PlayerUpdate, Round
        };

    }
}

#endif
