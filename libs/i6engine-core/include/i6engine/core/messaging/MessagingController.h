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
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_MESSAGINGCONTROLLER_H__
#define __I6ENGINE_CORE_MESSAGINGCONTROLLER_H__

#include <list>
#include <map>

#include "i6engine/utils/DoubleBufferQueue.h"

#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/subsystem/ModuleController.h"

#include "boost/thread/condition_variable.hpp"
#include "boost/thread/mutex.hpp"

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

namespace i6engine {
namespace core {

	class EngineCoreController;
	class MessageSubscriber;

	/**
	 * \class MessagingController
	 * \brief Central controller for messaging between subsystems and subsystems of different clients
	 *
	 * This class is responsible for accepting, queuing and delivering messages to subscribers. Furthermore an object that inherits from
	 * the super class MessageSubscriber may subscribe to a certain message type to receive messages of this type.
	 * Every object is allowed to send messages. To do so, create an instance of the Message class, add some variables and call deliverMessage:
	 * \code
	 * objMessage = new Message("type");
	 * objMessage->addVariable("value", 10);
	 * objMessage->addVariable("a_text", "here is a text");
	 * objMessage->addVariable("another_text", "here is another text");
	 * MessagingController::GetSingleton().deliverMessage(objMessage);
	 * \endcode
	 *
	 * To receive messages of a certain type, use the ISIXE_REGISTERMESSAGETYPE macro. There, the handling method for the message type has to be
	 * specified. This method will be called, when a message of this type arrives and should look like in the example below. Inside the
	 * method, use getVariable of the Message class as well as the macros RETURNIFVARIABLEISNULL and CONVERTVARIABLE to 'unpack' the message:
	 * \code
	 * ISIXE_REGISTERMESSAGETYPE("Type1", AudioController::Mailbox); // When a message of 'Type1' arrives, 'Mailbox' will be called
	 * ...
	 * // handling method
	 * void AudioController::Mailbox(Message * objMessage)
	 * {
	 * // unpack the variable 'value'
	 * int i;
	 * MessageVariable * objValue = objMessage->getVariable("value");
	 * RETURNIFVARIABLEISNULL(objValue);
	 * CONVERTVARIABLE(i, int, objValue);
	 * // do some stuff with i or unpack further variables of the message
	 * ...
	 * }
	 * \endcode
	 * \internal To get an overview on the usage and functionality of the whole messaging subsystem in German have a look at our wiki
	 * (topic: 'Messaging in der i6engine (Kurzbeschreibung)' )
	 *
	 * \note
	 * MessagingController.h defines the types:
	 * \li \c MessageTypeInfoList is an alias for std::list<const MessageTypeInfo *>
	 * \li \c MessageList is an alias for std::list<const Message *>
	 * \li \c MessageTypeDictionary is an alias for std::map<const std::string, MessageTypeInfoList *>
	 *
	 * MessagingController.h defines the macros:
	 * \li \c ISIXE_REGISTERMESSAGETYPE(strMessageTypeName, ptrMessageMethod) This macro wraps the registerMessageType method. It simplifies the use
	 * of this method by setting objMessageSubscriber to 'this' and binding the function pointer so that users can just type in the method name
	 * as second parameter.
	 * \li \c ISIXE_UNREGISTERMESSAGETYPE(strMessageTypeName) This macro wraps the unregisterMessageType method. It just sets objMessageSubscriber to
	 * 'this'.
	 *
	 * Have a look at \link i6engine here \endlink for an overview of typedef's in namespace %i6engine.
	 * See \ref macrodoc for a list of all globally defined macros.
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
		mutable boost::mutex _objDictionaryMutex;

		/**
		 * \brief queue containing messages for the controller
		 */
		utils::DoubleBufferQueue<Message::Ptr, true, false> _msgQueue;

		boost::thread * _deliverThread;

		boost::condition_variable _objCondExecutable;
		boost::mutex _objCondMut;
		boost::unique_lock<boost::mutex> _objCondUniqLock;

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
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_MESSAGINGCONTROLLER_H__ */

/**
 * @}
 */
