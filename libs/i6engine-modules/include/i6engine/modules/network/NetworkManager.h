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

#ifndef __I6ENGINE_MODULES_NETWORKMANAGER_H__
#define __I6ENGINE_MODULES_NETWORKMANAGER_H__

#include <map>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/messaging/IPKey.h"

#include "i6engine/api/GameMessage.h"

#include "i6engine/modules/network/NetworkErrors.h"

#include "boost/thread/mutex.hpp"

#include "m2etis/message/M2Message.h"
#include "m2etis/pubsub/DeliverCallbackInterface.h"

namespace m2etis {
namespace pubsub {
	enum ChannelName : uint16_t;
	class PubSubSystem;
} /* namespace pubsub */
} /* namespace m2etis */

namespace i6e {
namespace modules {

	class NetworkController;
	class NetworkMailbox;

	class NetworkManager : public m2etis::pubsub::BasicDeliverCallbackInterface<api::GameMessage> {
		friend class NetworkController;
		friend class NetworkErrors;
		friend class NetworkMailbox;

	private:
		/**
		 * \brief mailbox for the networking subsystem
		 */
		NetworkMailbox * _mailbox;

		bool _sendPing;
		int32_t _counterSent;
		int32_t _counterReceived;

		NetworkErrors _ne;

		m2etis::pubsub::PubSubSystem * _pubsub;

		uint64_t _lastTickTime;

		volatile bool _connectionFailed;

		std::map<uint16_t, uint32_t> _usedChannels;

		boost::mutex _pubSubLock;

		/**
		 * \brief constructor
		 */
		explicit NetworkManager(NetworkMailbox * mailbox);

		/**
		 * \brief destructor
		 */
		~NetworkManager();

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
		void publish(uint16_t channel, const api::GameMessage::Ptr & msg);

		/**
		 * \brief connects with given local port to the remote IP and the remote port
		 */
		bool connect(const core::IPKey & remote, const core::IPKey & local);

		/**
		 * \brief disconnects from a multiplayergame
		 */
		void disconnect();

		/**
		 * \brief receives the messages coming from the network
		 */
		void deliverCallback(const m2etis::message::M2Message<api::GameMessage>::Ptr msg) override;

		/**
		 * \brief called when connection to server failed
		 */
		void connectionFailed() { _connectionFailed = true; }

		/**
		 * \brief internal method, implements m2etis API
		 */
		void publishInternal(m2etis::pubsub::ChannelName channel, const m2etis::message::M2Message<api::GameMessage>::Ptr & msg) const;

		/**
		 * \brief enables or disables the sending of ping messages
		 *
		 * Default is off
		 */
		void enablePing(bool enabled);

		/**
		 * \brief Ticking the subsystem
		 */
		void Tick();

		/**
		 * \brief forbidden
		 */
		NetworkManager(const NetworkManager &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkManager & operator=(const NetworkManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_NETWORKMANAGER_H__ */

/**
 * @}
 */
