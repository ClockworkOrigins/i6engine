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

#include "i6engine/api/configs/NetworkConfig.h"

#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"

namespace i6engine {
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

} /* namespace network */
} /* namespace api */
} /* namespace i6engine */

BOOST_CLASS_EXPORT(i6engine::api::network::Administration_GameState_Create)
BOOST_CLASS_EXPORT(i6engine::api::network::Network_Ping_Update)
BOOST_CLASS_EXPORT(i6engine::api::network::Network_Pingtime_Update)
BOOST_CLASS_EXPORT(i6engine::api::network::Network_PingEnable_Update)
BOOST_CLASS_EXPORT(i6engine::api::network::Network_TimeSynchro_Update)
