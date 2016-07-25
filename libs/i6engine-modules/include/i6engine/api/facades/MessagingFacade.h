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
 * \addtogroup Messaging
 * @{
 */

#ifndef __I6ENGINE_API_MESSAGINGFACADE_H__
#define __I6ENGINE_API_MESSAGINGFACADE_H__

#include "i6engine/api/EngineController.h"
#include "i6engine/api/GameMessage.h"

#include "boost/bind.hpp"

#define ISIXE_REGISTERMESSAGETYPE(msgType, method, owner) i6e::api::EngineController::GetSingletonPtr()->getMessagingFacade()->registerMessageType(msgType, this, boost::bind(&method, owner, _1))
#define ISIXE_UNREGISTERMESSAGETYPE(msgType) i6e::api::EngineController::GetSingletonPtr()->getMessagingFacade()->unregisterMessageType(msgType, this)

namespace i6e {
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
		void registerMessageType(uint16_t msgType, core::MessageSubscriber * objMessageSubscriber, const std::function<void(GameMessage::Ptr)> & ptrMessageMethod);

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_MESSAGINGFACADE_H__ */

/**
 * @}
 */
