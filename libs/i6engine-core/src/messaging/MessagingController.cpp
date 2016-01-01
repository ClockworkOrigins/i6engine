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

#include "i6engine/core/messaging/MessagingController.h"

#include "i6engine/utils/Logger.h"

#ifdef ISIXE_WITH_PROFILING
	#include "i6engine/utils/Profiling.h"
#endif

#include "i6engine/core/messaging/MessageSubscriber.h"

#include "boost/thread.hpp"

namespace i6engine {
namespace core {

	MessagingController::MessagingController() : _objMessageTypeDictionary(), _objDictionaryMutex(), _deliverThread(nullptr), _condVar(), _condMutex(), _running(true) {
		_deliverThread = new boost::thread(boost::bind(&MessagingController::deliverMessages, this));
	}

	MessagingController::~MessagingController() {
		_running = false;
		_condVar.notify_all();
		_deliverThread->interrupt();
		_deliverThread->join();
		delete _deliverThread;
	}

	// Deliver all messages that are stored in the queue.
	void MessagingController::deliverMessages() {
		// Step 3
		// send Messages to the buffering method
		while (_running) {
			while (!_msgQueue.empty()) {
				deliverMessageToSubscribers(_msgQueue.poll());
			}
			std::unique_lock<std::mutex> ul(_condMutex);
			_condVar.wait(ul);
		}
	}

	void MessagingController::clear() {
		_msgQueue.clear();
	}

	void MessagingController::deliverMessageToSubscribers(const Message::Ptr & msg) const {
#ifdef ISIXE_WITH_PROFILING
		msg->insertTimestamp("MessagingController deliverMessageToSubscribers");
#endif /* ISIXE_WITH_PROFILING */
		uint16_t msgType = msg->getMessageType();

		boost::mutex::scoped_lock objScopeLock(_objDictionaryMutex);
		// Look up the message type in the dictionary (method suggested by Item 24 of Effective STL by Scott Meyers).
		auto it = _objMessageTypeDictionary.find(msgType);
		if (it != _objMessageTypeDictionary.end()) {
			// For each subscriber of the message type: Copy the message and store it in the local buffer of the receiver by calling receiveMessage.
			for (MessageSubscriber * msgSub : it->second) {
				msgSub->receiveMessage(msg);
			}
		} else {
			// if no one is subscribed to the message type, it's no error
			// uneccessary debugging messages
			ISIXE_LOG_INFO("Messaging", "Nobody registered for '" << msgType << "' maybe stop sending this message?\tSubtype is " << msg->getSubtype());
		}
	}

	void MessagingController::registerMessageType(uint16_t msgType, MessageSubscriber * subcriber) {
		boost::mutex::scoped_lock objScopeLock(_objDictionaryMutex);

		// Look up if the message type already exists in the dictionary (Suggested by Item 24 of Effective STL by Scott Meyers).
		auto itMsgType = _objMessageTypeDictionary.find(msgType);

		if (itMsgType != _objMessageTypeDictionary.end()) {
			// Look up if the method caller already subscribed for this message type.
			for (MessageSubscriber * sub : itMsgType->second) {
				if (sub == subcriber) {
					ISIXE_LOG_WARN("Messaging", "Message " << msgType << " already defined with the Message Subscriber");
					return;
				}
			}
			_objMessageTypeDictionary[msgType].push_back(subcriber);
		} else {
			// add the Subscriber to the list
			// operator[] will automatically create the new list
			_objMessageTypeDictionary[msgType].push_back(subcriber);
		}
	}

	void MessagingController::unregisterMessageType(uint16_t msgType, MessageSubscriber * subscriber) {
		boost::mutex::scoped_lock objScopeLock(_objDictionaryMutex);
		auto itMsgType = _objMessageTypeDictionary.find(msgType);
		// Look up if the message type exists in the dictionary (Suggested by Item 24 of Effective STL by Scott Meyers).
		if (itMsgType != _objMessageTypeDictionary.end()) {
			for (size_t i = 0; i < itMsgType->second.size(); ++i) {
				if (itMsgType->second[i] == subscriber) {
					itMsgType->second.erase(itMsgType->second.begin() + int(i));
				}
			}
			if (itMsgType->second.empty()) {
				_objMessageTypeDictionary.erase(itMsgType);
			}
		}
	}

	void MessagingController::deliverMessage(const Message::Ptr & msg) {
#ifdef ISIXE_WITH_PROFILING
		msg->insertTimestamp("MessagingController deliverMessage");
#endif /* ISIXE_WITH_PROFILING */
		// Step 1
		// entry point for other Threads
		// save Message
		_msgQueue.push(msg);
		// and than notify poll thread that there are new messages
		_condVar.notify_all();
	}

} /* namespace core */
} /* namespace i6engine */
