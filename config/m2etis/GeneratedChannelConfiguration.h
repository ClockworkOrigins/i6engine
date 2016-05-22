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

#ifndef M2ETIS_GENERATEDCHANNELCONFIGURATIONEXAMPLE_H
#define M2ETIS_GENERATEDCHANNELCONFIGURATIONEXAMPLE_H


}}
// Todo: refactor (namespace opened by ChannelTypeDefinitions.h closed)
// namespace m2etis pubsub closed because of the following includes:

#include "m2etis/config/GeneratedEventTypes.h"
#include "m2etis/config/DetMergeConfigTest.h"

namespace m2etis {
namespace pubsub {


typedef Channel<
    ChannelType<
        DirectBroadcastRouting<net::NetworkType<net::clockTCP> >
		, NullFilter<i6engine::api::GameMessage, net::NetworkType<net::clockTCP> >
		, NullOrder<net::NetworkType<net::clockTCP> >
		, NullDeliver<net::NetworkType<net::clockTCP> >
        , NullPersistence
        , NullValidity
		, NullPartition<net::NetworkType<net::clockTCP>, i6engine::api::GameMessage>
        , NullSecurity, NullRendezvous
    >,
	net::NetworkType<net::clockTCP>,
	i6engine::api::GameMessage
> I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCPType;

template <>
struct ChannelT<I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP>{
    ChannelT(const std::string & ip, const unsigned short port, const std::string & known_hostname, const unsigned short known_hostport, PubSubSystemEnvironment * pssi, std::vector<ChannelEventInterface *> & map, const std::vector<std::string> & rootList)
    {
        map.push_back(new I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCPType( I6ENGINE_Direct_Null_Null_Null_Null_Null_DirectBroadcast_Null_GameMessage_TCP, ip, port, known_hostname, known_hostport, pssi, rootList));
    }
};

#endif

