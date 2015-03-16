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

#ifndef __I6ENGINE_CORE_MESSAGESUBSCRIBER_H__
#define __I6ENGINE_CORE_MESSAGESUBSCRIBER_H__

#include <set>
#include <vector>

#include "i6engine/core/messaging/Message.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace core {

	class MessageTypeInfo;

	/**
	 * \brief Status of an 'Object'
	 */
	enum class IDStatus : uint16_t {
		NONE,		//! unknown (most likely not created yet)
		CREATED,	//! created
		DELETED		//! deleted (but it already existed)
	};

	/**
	 * \TODO Docu
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
	 * \li \c ReceivedMessage is an alias for struct { Message::Ptr message; const MessageTypeInfo * messagetypeinfo; }
	 * \li \c MessageVector is an alias for std::vector<ReceivedMessage>
	 *
	 * Have a look at \link i6engine here \endlink for an overview of typedef's in namespace %i6engine.
	 */
	class ISIXE_CORE_API MessageSubscriber {
	public:
		/**
		 * \brief Standard constructor
		 */
		MessageSubscriber() : _objMessageVectorMutex(), _objMessageVectorA(), _objMessageVectorB(), _objActiveMessageVector(&_objMessageVectorA), _objInActiveMessageVector(&_objMessageVectorB), _ptrMessageMethod(), _newCreatedIDs(), _existingObjects(), _waitingMsgs(), _objMessageListMutex() {
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
		 * \brief Calls deliverMessageInternal if the message should be delivered now, buffers it if not.
		 * \param msg Message to be be delivered.
		 */
		void buffer(const ReceivedMessagePtr & msg);

		/**
		 * \brief todo
		 */
		void reset();

		inline void addMethod(uint16_t msgType, const boost::function<void(const Message::Ptr &)> & ptrMessageMethod) {
			_ptrMessageMethod[msgType] = ptrMessageMethod;
		}

		inline void removeMethod(uint16_t msgType) {
			_ptrMessageMethod.erase(msgType);
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

	private:
		/**
		 * \brief This method really delivers the Message without buffering it anymore.
		 * \param msg Message to be delivered.
		 */
		void deliverMessageInternal(const ReceivedMessagePtr & msg);

		/**
		 * \brief vector containing all new notified IDs since the last tick happened
		 */
		std::vector<int64_t> _newCreatedIDs; // list of all IDs that got created since the last tick

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
} /* namespace i6engine */

/**
 * @}
 */

#endif /* __I6ENGINE_CORE_MESSAGESUBSCRIBER_H__ */
