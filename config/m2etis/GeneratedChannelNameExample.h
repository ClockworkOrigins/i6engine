/**
 Copyright 2013 FAU (Friedrich Alexander University of Erlangen-Nuremberg)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
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
