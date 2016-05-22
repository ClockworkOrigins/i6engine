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
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_MESSAGINGCONTROLLER_H__
#define __I6ENGINE_CORE_MESSAGINGCONTROLLER_H__

#include <condition_variable>
#include <list>
#include <map>
#include <mutex>

#include "i6engine/utils/DoubleBufferQueue.h"

#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/subsystem/ModuleController.h"

/**
 * \page page_messagingSystem Messaging System
 *
 * \section sec_core_message_create Creating a Message
 * a thread creates a Message and specifies a type for this Message\n
 * the thread can add several variables to that Message\n
 * than the thread sends the message to the MessageProcessor\n
 * \n
 * Whenever the Tick() method of the MessageProcessor is executed, it takes all arrived Messages, determinds all MessageSubscribers that registered for this kind of Message and delivers the Message.\n
 * \n
 * The MessageSubscriber again buffers all Messages until the method processMessages() is called by the underlying thread. \n
 * Than it loops over all stored Messages and calls the methods, that are specified for that type of Message. \n
 *
 * \section sec_core_message Message
 * All components in the GameEngine communicate via Messages.
 * A message must derive from the class Message defined in Message.h
 * Each Message has a Typ and a list of Pairs (Key, Value),
 * where Key must be unique and Value can be a any primitive type or a std::string
 *
 * \section sec_core_messagesubscriber MessageSubscriber
 * The MessageSubscriber manages all incoming Messages for a certain SubSystem.
 * It is registered at the MessageProcessor for a certain type of Message.
 * Whenever processMessages() is called, for every arrived Message, the method specified for this type of Message is called.
 *
 * \section sec_core_messageprocessor MessageProcessor
 * The MessageProcessor buffers all incoming Messages and whenever Tick() is called, it delivers the Messages to all registered MessageSubscribers.
 *
 * To register for such a Messagetyp, use the macro
 * \code
 * ISIXE_REGISTERMESSAGETYPE("type", method[, parameter])
 * // to unsubsribe for a specific Type, use the macro
 * ISIXE_UNREGISTERMESSAGETYPE("type")
 * \endcode
 *
 * \image html Core_Messaging.png
 */

namespace i6e {
namespace core {

	class EngineCoreController;
	class MessageSubscriber;

	/**
	 * \class MessagingController
	 * \brief Central controller for messaging between subsystems and subsystems of different clients
	 */
	class ISIXE_CORE_API MessagingController {
	public:
		/**
		 * \brief Standard constructor.
		 */
		MessagingController();

		/**
		 * \brief
		 */
		~MessagingController();

		/**
		 * \brief This method registers a message type with the given message subscriber and the given Method. It is normally called via the macro ISIXE_REGISTERMESSAGETYPE.
		 *
		 * \todo Calling this method directly allows it, that objects let other objects subscribe to certain message types.
		 * \param strMessageType Name of the new message type.
		 * \param objMessageSubscriber The new receiver of messages of the given type (should be the caller by itself).
		 * \param bolNetworking Flag if the message type should sent over the network.
		 */
		void registerMessageType(uint16_t messageType, MessageSubscriber * objMessageSubscriber);

		/**
		 * \brief This method unregisters a message type of the given message subscriber.
		 * \param strMessageType Name of the new message type.
		 * \param objMessageSubscriber Receiver to be deleted from the subscriber list.
		 */
		void unregisterMessageType(uint16_t messageType, MessageSubscriber * objMessageSubscriber);

		/**
		 * \brief This method sends a message.
		 * \param The message to be sent.
		 */
		void deliverMessage(const Message::Ptr & msg);

		/**
		 * \brief removes all messages in buffer
		 */
		void clear();

	private:
		/**
		 * \brief This method delivers the messages.
		 */
		void deliverMessages();

		/**
		 * \brief Entry point for other Thread to deliver a message.
		 * \param msg Message to be delivered.
		 */
		void deliverMessageToSubscribers(const Message::Ptr & msg) const;

		/**
		 * \brief
		 */
		std::map<uint16_t, std::vector<MessageSubscriber *>> _objMessageTypeDictionary;

		/**
		 * \brief
		 */
		mutable std::mutex _dictionaryMutex;

		/**
		 * \brief queue containing messages for the controller
		 */
		utils::DoubleBufferQueue<Message::Ptr, true, false> _msgQueue;

		boost::thread * _deliverThread;

		std::condition_variable _condVar;
		std::mutex _condMutex;

		bool _running;

		/**
		 * \brief forbidden
		 */
		MessagingController(const MessagingController &) = delete;

		/**
		 * \brief forbidden
		 */
		MessagingController & operator=(const MessagingController &) = delete;
	};

} /* namespace core */
} /* namespace i6e */

#endif /* __I6ENGINE_CORE_MESSAGINGCONTROLLER_H__ */

/**
 * @}
 */
