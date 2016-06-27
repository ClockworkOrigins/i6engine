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

#include "i6engine/core/messaging/MessagingController.h"

#include <thread>

#include "i6engine/utils/Logger.h"

#ifdef ISIXE_WITH_PROFILING
	#include "i6engine/utils/Profiling.h"
#endif

#include "i6engine/core/messaging/MessageSubscriber.h"

namespace i6e {
namespace core {

	MessagingController::MessagingController() : _objMessageTypeDictionary(), _dictionaryMutex(), _deliverThread(nullptr), _condVar(), _condMutex(), _running(true) {
		_deliverThread = new std::thread(std::bind(&MessagingController::deliverMessages, this));
	}

	MessagingController::~MessagingController() {
		_running = false;
		{
			std::unique_lock<std::mutex> ul(_condMutex);
			_condVar.notify_all();
		}
		_deliverThread->join();
		delete _deliverThread;
	}

	// Deliver all messages that are stored in the queue.
	void MessagingController::deliverMessages() {
		// Step 3
		// send Messages to the buffering method
		while (_running) {
			while (!_msgQueue.empty()) {
				i6e::core::Message::Ptr msg;
				_msgQueue.poll(msg); // Daniel: no error check necessary as it is polled just from one thread
				deliverMessageToSubscribers(msg);
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

		std::lock_guard<std::mutex> lg(_dictionaryMutex);
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
		std::lock_guard<std::mutex> lg(_dictionaryMutex);

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
		std::lock_guard<std::mutex> lg(_dictionaryMutex);
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
		std::unique_lock<std::mutex> ul(_condMutex);
		_msgQueue.push(msg);
		// and than notify poll thread that there are new messages
		_condVar.notify_all();
	}

} /* namespace core */
} /* namespace i6e */
