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

/**
 * \addtogroup Network
 * @{
 */

#ifndef __I6ENGINE_API_NETWORKFACADE_H__
#define __I6ENGINE_API_NETWORKFACADE_H__

#include "i6engine/core/messaging/IPKey.h"

#include "i6engine/api/GameMessage.h"

#include "boost/function.hpp"

namespace i6e {
namespace api {

	class ISIXE_MODULES_API NetworkFacade {
	public:
		NetworkFacade();
		~NetworkFacade();

		/**
		 * \brief returns the IP
		 */
		inline core::IPKey getIP() const { return _ownIP; }

		/**
		 * \brief returns the IP without Port as a string
		 */
		inline std::string getSimpleIP() const { return _IP; }

		/**
		 * \brief connects with given local port to the remote IP and the remote port
		 */
		bool connect(const core::IPKey remoteIP, const uint16_t localPort);

		bool listen(const uint16_t localPort);

		/**
		 * \brief disconnects from a multiplayergame
		 */
		void disconnect();

		/**
		 * \brief Enables the ping-time measure
		 */
		void enablePing(bool enabled);

		/**
		 * \brief subscribes on given channel
		 */
		void subscribe(uint16_t channel); // TODO: (Daniel) add second subscribe method registering a filter predicate too

		/**
		 * \brief unsubscribes from given channel
		 */
		void unsubscribe(uint16_t channel); // TODO: (Daniel) add second unsubscribe method registering a filter predicate too

		/**
		 * \brief publishes on given channel
		 */
		void publish(uint16_t channel, const GameMessage::Ptr & msg);

		/**
		 * \brief registers a callback for subscribe, call only by networking subsystem
		 */
		void registerSubscribeCallback(const boost::function<void(uint16_t)> & f);

		/**
		 * \brief registers a callback for unsubscribe, call only by networking subsystem
		 */
		void registerUnsubscribeCallback(const boost::function<void(uint16_t)> & f);

		/**
		 * \brief registers a callback for publish, call only by networking subsystem
		 */
		void registerPublishCallback(const boost::function<void(uint16_t, const GameMessage::Ptr &)> & f);

		/**
		 * \brief registers a callback for connect, call only by networking subsystem
		 */
		void registerConnectCallback(const boost::function<bool(const core::IPKey &, const core::IPKey &)> & f);

		/**
		 * \brief registers a callback for disconnect, call only by networking subsystem
		 */
		void registerDisconnectCallback(const boost::function<void(void)> & f);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

	private:
		/**
		 * \brief represents the IP of this node
		 */
		core::IPKey _ownIP;

		/**
		 * \brief holds state whether a subscribe callback is set or not
		 */
		bool _subscribeInit;

		/**
		 * \brief holds state whether a unsubscribe callback is set or not
		 */
		bool _unsubscribeInit;

		/**
		 * \brief holds state whether a publish callback is set or not
		 */
		bool _publishInit;

		/**
		 * \brief callback for subscribe
		 */
		boost::function<void(uint16_t)> _subscribe;

		/**
		 * \brief callback for subscribe
		 */
		boost::function<void(uint16_t)> _unsubscribe;

		/**
		 * \brief callback for subscribe
		 */
		boost::function<void(uint16_t, const GameMessage::Ptr &)> _publish;

		/**
		 * \brief holds state whether a connect callback is set or not
		 */
		bool _connectInit;

		/**
		 * \brief holds state whether a disconnect callback is set or not
		 */
		bool _disconnectInit;

		/**
		 * \brief callback for connect
		 */
		boost::function<bool(const core::IPKey &, const core::IPKey &)> _connect;

		/**
		 * \brief callback for connect
		 */
		boost::function<void(void)> _disconnect;

		bool _connected;

		/**
		 * \brief stores the IP
		 * This is used to create _ownIP
		 */
		std::string _IP;

		/**
		 * \brief forbidden
		 */
		NetworkFacade(const NetworkFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkFacade & operator= (const NetworkFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_NETWORKFACADE_H__ */

/**
 * @}
 */
