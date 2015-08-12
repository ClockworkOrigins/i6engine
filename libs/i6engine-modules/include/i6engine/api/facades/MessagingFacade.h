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
 * \addtogroup Messaging
 * @{
 */

#ifndef __I6ENGINE_API_MESSAGINGFACADE_H__
#define __I6ENGINE_API_MESSAGINGFACADE_H__

#include "i6engine/api/EngineController.h"
#include "i6engine/api/GameMessage.h"

#include "boost/bind.hpp"

#define ISIXE_REGISTERMESSAGETYPE(msgType, method, owner) i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->registerMessageType(msgType, this, boost::bind(&method, owner, _1))
#define ISIXE_UNREGISTERMESSAGETYPE(msgType) i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->unregisterMessageType(msgType, this)

namespace i6engine {
namespace core {
	class MessageSubscriber;
	class MessagingController;
} /* namespace core */
namespace api {

	/**
	 * \brief
	 */
	class ISIXE_MODULES_API MessagingFacade {
		friend class EngineController;

	public:
		/**
		 * \brief
		 */
		MessagingFacade();

		/**
		 * \brief
		 */
		~MessagingFacade();

		/**
		 * \brief sends a message
		 */
		void deliverMessage(const GameMessage::Ptr & msg);

		/**
		 * \brief This method registers a message type with the given message subscriber and the given Method. It is normally called via the macro ISIXE_REGISTERMESSAGETYPE.
		 *
		 * Calling this method directly allows it, that objects let other objects subscribe to certain message types.
		 * \param strMessageType Name of the new message type.
		 * \param objMessageSubscriber The new receiver of messages of the given type (should be the caller by itself).
		 * \param ptrMessageMethod The name of the method that will be called when a message of a certain type arrives at the receiver.
		 */
		void registerMessageType(uint16_t msgType, core::MessageSubscriber * objMessageSubscriber, const boost::function<void(GameMessage::Ptr)> & ptrMessageMethod);

		/**
		 * \brief This method unregisters a message type of the given message subscriber.
		 * \param strMessageType Name of the new message type.
		 * \param objMessageSubscriber Receiver to be deleted from the subscriber list.
		 */
		void unregisterMessageType(uint16_t msgType, core::MessageSubscriber * objMessageSubscriber);

	private:
		core::MessagingController * _messagingController;

		/**
		 * \brief registers the callback in MessagingController, called from EngineController only
		 */
		void registerMessagingController(core::MessagingController * ctrl);

		/**
		 * \brief forbidden
		 */
		MessagingFacade(const MessagingFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		MessagingFacade & operator=(const MessagingFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MESSAGINGFACADE_H__ */

/**
 * @}
 */
