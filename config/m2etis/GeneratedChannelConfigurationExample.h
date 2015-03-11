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

