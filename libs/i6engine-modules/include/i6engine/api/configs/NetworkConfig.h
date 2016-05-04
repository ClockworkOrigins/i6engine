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
 * \addtogroup Network
 * @{
 */

#ifndef __I6ENGINE_API_NETWORKCONFIG_H__
#define __I6ENGINE_API_NETWORKCONFIG_H__

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/GameMessageStruct.h"

#include "boost/shared_ptr.hpp"

namespace i6e {
namespace api {
namespace network {
	enum NetworkMessageTypes {
		NetReset = 0,
		NetConnect,
		NetGameState,
		NetPingRequest,
		NetPingAnswer,
		NetPingTime,
		NetPingEnable,
		NetRefused,
		NetTimeSynchro,
		ChatMessage,
		NetworkMessageTypesCount
	};

	/**
	 * \brief message containing a list of all GameObjects the server contains
	 */
	typedef struct ISIXE_MODULES_API Administration_GameState_Create : GameMessageStruct {
		std::vector<GameMessage::Ptr> messages;
		core::IPKey receiver;
		Administration_GameState_Create() {}
		Administration_GameState_Create(const core::IPKey & r, const std::vector<GameMessage::Ptr> & msgs);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & messages;
			ar & receiver;
		}
		Administration_GameState_Create * copy() { return new Administration_GameState_Create(*this); }
	} Administration_GameState_Create;

	/**
	 * \brief sends ping message to all clients containing send time
	 */
	typedef struct ISIXE_MODULES_API Network_Ping_Update : GameMessageStruct {
		uint64_t time;
		core::IPKey receiver;
		Network_Ping_Update() {}
		Network_Ping_Update(const core::IPKey & r, uint64_t time);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & time;
			ar & receiver;
		}
		Network_Ping_Update * copy() { return new Network_Ping_Update(*this); }
	} Network_Ping_Update;

	/**
	 * \brief synchronization message to update players ping visualization
	 */
	typedef struct ISIXE_MODULES_API Network_Pingtime_Update : GameMessageStruct {
		uint64_t ping;
		core::IPKey client;
		Network_Pingtime_Update() {}
		Network_Pingtime_Update(const core::IPKey & c, uint64_t p);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & ping;
			ar & client;
		}
		Network_Pingtime_Update * copy() { return new Network_Pingtime_Update(*this); }
	} Network_Pingtime_Update;

	/**
	 * \brief synchronization message to update application time at client
	 */
	typedef struct ISIXE_MODULES_API Network_TimeSynchro_Update : GameMessageStruct {
		uint64_t ping;
		uint64_t time;
		core::IPKey receiver;
		Network_TimeSynchro_Update() {
		}
		Network_TimeSynchro_Update(const core::IPKey & r, uint64_t p, uint64_t t);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & ping;
			ar & time;
			ar & receiver;
		}
		Network_TimeSynchro_Update * copy() {
			return new Network_TimeSynchro_Update(*this);
		}
	} Network_TimeSynchro_Update;

	/**
	 * \brief sets activation state of pinging
	 */
	typedef struct ISIXE_MODULES_API Network_PingEnable_Update : GameMessageStruct {
		bool enable;
		Network_PingEnable_Update() {}
		Network_PingEnable_Update(bool b);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & enable;
		}
		Network_PingEnable_Update * copy() { return new Network_PingEnable_Update(*this); }
	} Network_PingEnable_Update;

	/**
	 * \brief sends a text message to other subscribers
	 */
	typedef struct ISIXE_MODULES_API Network_ChatMessage : GameMessageStruct {
		std::string text;
		Network_ChatMessage() : GameMessageStruct(), text() {
		}
		Network_ChatMessage(const std::string & t);
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & boost::serialization::base_object<GameMessageStruct>(*this);
			ar & text;
		}
		Network_ChatMessage * copy() {
			return new Network_ChatMessage(*this);
		}
	} Network_ChatMessage;

} /* namespace network */
} /* namespace api */
} /* namespace i6e */

BOOST_CLASS_EXPORT_KEY(i6e::api::network::Administration_GameState_Create)
BOOST_CLASS_EXPORT_KEY(i6e::api::network::Network_Ping_Update)
BOOST_CLASS_EXPORT_KEY(i6e::api::network::Network_Pingtime_Update)
BOOST_CLASS_EXPORT_KEY(i6e::api::network::Network_PingEnable_Update)
BOOST_CLASS_EXPORT_KEY(i6e::api::network::Network_TimeSynchro_Update)
BOOST_CLASS_EXPORT_KEY(i6e::api::network::Network_ChatMessage)

#endif /* __I6ENGINE_API_NETWORKCONFIG_H__ */

/**
 * @}
 */
