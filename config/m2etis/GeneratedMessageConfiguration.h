/**
 Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)

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

#ifndef m2etis_MessageConfigurationExample_h
#define m2etis_MessageConfigurationExample_h

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/config/DetMergeConfigTest.h"
#include "boost/serialization/serialization.hpp"
#include "boost/serialization/level.hpp"
#include "boost/serialization/tracking.hpp"

BOOST_CLASS_EXPORT_GUID(i6engine::core::Message, "0")
BOOST_CLASS_IMPLEMENTATION(i6engine::core::Message, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6engine::core::MessageStruct, "1")
BOOST_CLASS_IMPLEMENTATION(i6engine::core::MessageStruct, boost::serialization::object_serializable)
//BOOST_CLASS_EXPORT_GUID(i6engine::api::GameMessage, "2")
//BOOST_CLASS_IMPLEMENTATION(i6engine::api::GameMessage, boost::serialization::object_serializable)
//BOOST_CLASS_EXPORT_GUID(i6engine::api::GameMessageStruct, "3")
//BOOST_CLASS_IMPLEMENTATION(i6engine::api::GameMessageStruct, boost::serialization::object_serializable)

BOOST_CLASS_EXPORT_GUID(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::clockTCP>>, "4")
BOOST_CLASS_IMPLEMENTATION(m2etis::message::DirectBroadcastRoutingInfo<m2etis::net::NetworkType<m2etis::net::clockTCP>>, boost::serialization::object_serializable)

BOOST_CLASS_EXPORT_GUID(m2etis::message::NullFilterInfo<i6engine::api::GameMessage>, "5") // TODO: (Roland) check if necessary
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullFilterInfo<i6engine::api::GameMessage>, boost::serialization::object_serializable)

BOOST_CLASS_EXPORT_GUID(m2etis::message::NullOrderInfo, "6")
BOOST_CLASS_IMPLEMENTATION(m2etis::message::NullOrderInfo, boost::serialization::object_serializable)

// Direct / Null TCP

typedef m2etis::message::InternalMessage<m2etis::net::NetworkType<m2etis::net::clockTCP>, m2etis::pubsub::ChannelType<
m2etis::pubsub::routing::DirectBroadcastRouting<m2etis::net::NetworkType<m2etis::net::clockTCP>>
, m2etis::pubsub::filter::NullFilter<i6engine::api::GameMessage, m2etis::net::NetworkType<m2etis::net::clockTCP> >
, m2etis::pubsub::order::NullOrder<m2etis::net::NetworkType<m2etis::net::clockTCP>>
, m2etis::pubsub::deliver::NullDeliver<m2etis::net::NetworkType<m2etis::net::clockTCP>>
, m2etis::pubsub::persistence::NullPersistence
, m2etis::pubsub::validity::NullValidity
, m2etis::pubsub::partition::NullPartition<m2etis::net::NetworkType<m2etis::net::clockTCP>, i6engine::api::GameMessage>
, m2etis::pubsub::security::NullSecurity, m2etis::pubsub::rendezvous::NullRendezvous>, i6engine::api::GameMessage> Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCPMessage;
BOOST_CLASS_EXPORT_GUID(Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCPMessage, "7")
BOOST_CLASS_IMPLEMENTATION(Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCPMessage, boost::serialization::object_serializable)

#endif
