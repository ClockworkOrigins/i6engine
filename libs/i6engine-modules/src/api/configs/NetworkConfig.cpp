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

#include "i6engine/api/configs/NetworkConfig.h"

#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"

namespace i6e {
namespace api {
namespace network {

	Administration_GameState_Create::Administration_GameState_Create(const core::IPKey & r, const std::vector<GameMessage::Ptr> & msgs) : GameMessageStruct(), messages(msgs), receiver(r) {}

	Network_Ping_Update::Network_Ping_Update(const core::IPKey & r, uint64_t t) : GameMessageStruct(), time(t), receiver(r) {
	}

	Network_Pingtime_Update::Network_Pingtime_Update(const core::IPKey & cl, uint64_t p) : GameMessageStruct(), ping(p), client(cl) {}

	Network_PingEnable_Update::Network_PingEnable_Update(bool b) : GameMessageStruct(), enable(b) {
	}

	Network_TimeSynchro_Update::Network_TimeSynchro_Update(const core::IPKey & r, uint64_t p, uint64_t t) : GameMessageStruct(), ping(p), time(t), receiver(r) {
	}

	Network_ChatMessage::Network_ChatMessage(const std::string & t) : GameMessageStruct(), text(t) {
	}

} /* namespace network */
} /* namespace api */
} /* namespace i6e */

BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::network::Administration_GameState_Create)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::network::Network_Ping_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::network::Network_Pingtime_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::network::Network_PingEnable_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::network::Network_TimeSynchro_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::network::Network_ChatMessage)
