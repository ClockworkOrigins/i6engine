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

#ifndef __I6ENGINE_CORE_MESSAGESUBSCRIBER_H__
#define __I6ENGINE_CORE_MESSAGESUBSCRIBER_H__

#include <set>
#include <vector>

#include "i6engine/core/messaging/Message.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

namespace i6e {
namespace core {

	class MessageTypeInfo;

	/**
	 * \brief Status of an ID
	 */
	enum class IDStatus : uint16_t {
		NONE,		//! unknown (most likely not created yet)
		CREATED,	//! created
		DELETED		//! deleted (but it already existed)
	};

	/**
	 * \brief a received message within a MessageSubscriber
	 */
	struct ReceivedMessage {
		Message::Ptr message;
		std::set<int64_t> waitingFor;

		ReceivedMessage(const Message::Ptr & msg) : message(msg), waitingFor() {
		}
	};
	typedef boost::shared_ptr<ReceivedMessage> ReceivedMessagePtr;
	typedef std::vector<ReceivedMessagePtr> MessageVector;

	/**
	 * \class MessageSubscriber
	 * \brief Interface for every class that wants to subscribe to the messaging system.
	 *
	 * All instances of the classes SubSystem and GOComponent inherit from this class.
	 *
	 * \note
	 * MessageSubscriber.h defines the types:
	 * \li \c MessageVector is an alias for std::vector<ReceivedMessage>
	 *
	 * Have a look at \link i6engine here \endlink for an overview of typedef's in namespace %i6engine.
	 */
	class ISIXE_CORE_API MessageSubscriber {
	public:
		/**
		 * \brief Standard constructor
		 */
		MessageSubscriber() : _objMessageVectorMutex(), _objMessageVectorA(), _objMessageVectorB(), _objActiveMessageVector(&_objMessageVectorA), _objInActiveMessageVector(&_objMessageVectorB), _ptrMessageMethod(), _newCreatedIDs(), _newDeletedIDs(), _existingObjects(), _waitingMsgs(), _objMessageListMutex() {
		}

		/**
		 * \brief Destructor.
		 */
		virtual ~MessageSubscriber() {
		}

		/**
		 * \brief This method is called from the MessagingController. It stores incoming messages in a local buffer.
		 * \param objMessage The message to be stored in the buffer.
		 */
		void receiveMessage(const Message::Ptr & msg);

		/**
		 * \brief This method goes through the local message buffer and calls the appropriate method of the receiver.
		 *
		 * It is called prior to a tick in the main loop of every subsystem. This class must not be called explicitely. Subject to future refactoring to private scope.
		 */
		virtual void processMessages();

		/**
		 * \brief This method notifies the MessagingController that Messages waiting for this id can now be delivered.
		 * \param id The new id.
		 */
		void notifyNewID(const int64_t id);

		/**
		 * \brief This method notifies the MessagingController that Messages waiting for this id mustn't be forwarded anymore
		 * \param id The new id.
		 */
		void notifyDeletedID(const int64_t id);

		/**
		 * \brief Calls deliverMessageInternal if the message should be delivered now, buffers it if not.
		 * \param msg Message to be be delivered.
		 */
		void buffer(const ReceivedMessagePtr & msg);

		/**
		 * \brief todo
		 */
		void reset();

		/**
		 * \brief adds a method for given message type where messages shall be delivered
		 */
		inline void addMethod(uint16_t msgType, const boost::function<void(const Message::Ptr &)> & ptrMessageMethod) {
			_ptrMessageMethod[msgType] = ptrMessageMethod;
		}

		/**
		 * \brief removes method for given message type
		 */
		inline void removeMethod(uint16_t msgType) {
			_ptrMessageMethod.erase(msgType);
			boost::mutex::scoped_lock objScopeLock(_objMessageVectorMutex);
			MessageVector result;
			for (auto & rm : *_objActiveMessageVector) {
				if (rm->message->getMessageType() != msgType) {
					result.push_back(rm);
				}
			}
			_objActiveMessageVector->clear();
			for (auto & rm : result) {
				_objActiveMessageVector->push_back(rm);
			}
		}

	protected:
		/**
		 * \brief This method will swap the double message buffer. For synchronization reasons.
		 */
		void swapMessageBuffer();

		/**
		 * \brief This method delivers all waiting Messages that can now be delivered.
		 */
		bool updateBuffer();

		boost::mutex _objMessageVectorMutex;
		MessageVector _objMessageVectorA;
		MessageVector _objMessageVectorB;
		MessageVector * _objActiveMessageVector;
		MessageVector * _objInActiveMessageVector;

		std::map<uint16_t, boost::function<void(const Message::Ptr &)>> _ptrMessageMethod;

		/**
		 * \brief This method really delivers the Message without buffering it anymore.
		 * \param msg Message to be delivered.
		 */
		virtual void deliverMessageInternal(const ReceivedMessagePtr & msg);

	private:
		/**
		 * \brief vector containing all new notified IDs since the last tick happened
		 */
		std::vector<int64_t> _newCreatedIDs; // list of all IDs that got created since the last tick

		/**
		 * \brief vector containing all new deleted IDs since the last tick happened
		 */
		std::vector<int64_t> _newDeletedIDs;

		/**
		 * \brief list of all IDs being registered
		 */
		std::map<int64_t, IDStatus> _existingObjects; // map for all GameObjects and their status TODO: remove ids deleted long ago?

		/**
		 * \brief contains buffered messages
		 * key: <i>int</i> specifying the ID on which the messages are waiting
		 * value: <i>std::vector<Message::Ptr></i> a vector containing all messages that wait for the ID
		 */
		std::map<int64_t, std::vector<ReceivedMessagePtr>> _waitingMsgs;

		/**
		 * \brief lock for notifies
		 */
		mutable boost::mutex _objMessageListMutex;

		/**
		 * \brief lock for buffering methods
		 */
		mutable boost::mutex _bufferLock;

		/**
		 * \brief forbidden
		 */
		MessageSubscriber(const MessageSubscriber &) = delete;

		/**
		 * \brief forbidden
		 */
		MessageSubscriber & operator=(const MessageSubscriber &) = delete;
	};

} /* namespace core */
} /* namespace i6e */

#endif /* __I6ENGINE_CORE_MESSAGESUBSCRIBER_H__ */

/**
 * @}
 */
