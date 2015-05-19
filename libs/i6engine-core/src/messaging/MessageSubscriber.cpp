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

#include "i6engine/core/messaging/MessageSubscriber.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/messaging/MessageStruct.h"
#include "i6engine/core/messaging/MessageTypes.h"

#include "boost/make_shared.hpp"

namespace i6engine {
namespace core {

	void MessageSubscriber::receiveMessage(const Message::Ptr & msg) {
#ifdef ISIXE_WITH_PROFILING
		msg->getContent()->insertTimestamp("MessageSubscriber receiveMessage");
#endif /* ISIXE_WITH_PROFILING */
		buffer(boost::make_shared<ReceivedMessage>(msg));
	}

	void MessageSubscriber::swapMessageBuffer() {
		boost::mutex::scoped_lock objScopeLock(_objMessageVectorMutex);
		std::swap(_objActiveMessageVector, _objInActiveMessageVector);
	}

	void MessageSubscriber::buffer(const ReceivedMessagePtr & msg) {
#ifdef ISIXE_WITH_PROFILING
		msg->message->insertTimestamp("MessageSubscriber buffer");
#endif /* ISIXE_WITH_PROFILING */
		// Step 4
		// Determine whether to buffer or not
		int64_t id = msg->message->getContent()->_id;
		int64_t waitForId = msg->message->getContent()->_waitForId;
		Method method = msg->message->getMethod();

		// status of object with id = id
		IDStatus statusID = IDStatus::NONE;
		// status of object with id = waitForId
		IDStatus statusWaitID = IDStatus::NONE;

		// if buffer() is called frequently from many different threads, it may be better to release it after obtaining the status and
		// reaquire it later on
		// however for a single thread buffering, aquiring twice is very slow
		boost::mutex::scoped_lock sl(_bufferLock);

		if (id != -1 && _existingObjects.find(id) != _existingObjects.end()) {
			statusID = _existingObjects.at(id);
		}
		if (waitForId != -1 && _existingObjects.find(waitForId) != _existingObjects.end()) {
			statusWaitID = _existingObjects.at(waitForId);
		}
		if (id != -1 && _existingObjects.find(id) != _existingObjects.end() && _existingObjects[id] == IDStatus::DELETED && method != Method::Delete) {
			// Object already deleted, only deliver Messages to clean up remaining parts
			return;
		}

		if (statusWaitID == IDStatus::DELETED && method != Method::Delete) {
			return;
		}

		// id exists (we don't need to wait) iff
		//	* id not set
		//	* we'll create the id
		//	* the id is already created
		bool idEx = (id == -1 || method == Method::Create || statusID == IDStatus::CREATED);
		bool waitIdEx = (waitForId == -1 || statusWaitID == IDStatus::CREATED);
		if (idEx && waitIdEx) {
			// object(s) already exists
			deliverMessageInternal(msg);
		} else {
			// object(s) doesn't exist yet
			if (!idEx) {
				if (method == Method::Delete) {
					if (statusID == IDStatus::DELETED) {
						deliverMessageInternal(msg); // if several delete messages are used for one ID // FIXME: (Michael) is this still possible? Can we change it?
						// skip to avoid double delivery. Only possible because code at the end is
						// method==Method::Create and here ist method==Method::Delete
						return;
					} else {
						msg->waitingFor.insert(id);
						_waitingMsgs[id].push_back(msg); // object not yet created
					}
				} else {
					msg->waitingFor.insert(id);
					_waitingMsgs[id].push_back(msg);
				}
			}
			if (!waitIdEx) {
				if (method == Method::Delete && statusWaitID == IDStatus::DELETED) {
					deliverMessageInternal(msg); // if several delete messages are used for one ID
					// skip to avoid double delivery. Only possible because code at the end is
					// method==Method::Create and here ist method==Method::Delete
					return;
				} else {
					msg->waitingFor.insert(waitForId);
					_waitingMsgs[waitForId].push_back(msg);
				}
			}
		}
		if (method == Method::Create) {
			if (id != -1 && statusID == IDStatus::NONE && (waitForId == -1 || statusWaitID == IDStatus::CREATED)) {
				notifyNewID(id);
			}
		}
	}

	bool MessageSubscriber::updateBuffer() {
		// Step 6
		// determind all Messages that can now be delivered
		_objMessageListMutex.lock();
		std::vector<int64_t> ids = _newCreatedIDs;
		_newCreatedIDs.clear();
		_objMessageListMutex.unlock();

		bool notify = false;

		boost::mutex::scoped_lock sl(_bufferLock);

		for (int64_t i : ids) {
			if (_existingObjects.find(i) != _existingObjects.end()) {
				continue;
			}
			_existingObjects[i] = IDStatus::CREATED;

			// CAUTION: some messages in _waitingMsgs[i] could be deleted when msg is a Delete message
			// but only messages behind the current one
			// using an iterator will break, using an positioncounter works
			for (size_t msgNr = 0; msgNr < _waitingMsgs[i].size(); ++msgNr) {
				ReceivedMessagePtr msg = _waitingMsgs[i][msgNr];
				int64_t id = msg->message->getContent()->_id;
				int64_t waitForId = msg->message->getContent()->_waitForId;
				// delete Msg from other buffer
				msg->waitingFor.erase(i);
				if (id == i) { // this message waited for the Object 'id' to be created
					if (waitForId == -1) {
						deliverMessageInternal(msg);
					} else {
						if (msg->waitingFor.find(waitForId) == msg->waitingFor.end()/*findWaiting(msg, waitForId) == -1*/) {
							auto it = _existingObjects.find(waitForId);
							if (it != _existingObjects.end()) {
								if (it->second != IDStatus::DELETED) {
									deliverMessageInternal(msg);
								}
							} else {
								// ID not in existing map, but it should have been created
								ISIXE_LOG_FATAL("MessageSubscriber", "ID Lifetimecycle corrupted");
							}
						} else {
							// still waiting for another id
						}
					}
				} else { // this message waited for the Object 'waitForId' to be created
					if (id == -1 || msg->message->getMethod() == Method::Create) {
						// no more waiting needed
						deliverMessageInternal(msg);
					} else {
						if (msg->waitingFor.find(id) == msg->waitingFor.end()/*findWaiting(msg, id) == -1*/) {
							auto it = _existingObjects.find(id);
							if (it != _existingObjects.end()) {
								if (it->second != IDStatus::DELETED) {
									deliverMessageInternal(msg);
								}
							} else {
								// ID not in existing map, but it should have been created
								ISIXE_LOG_FATAL("MessageSubscriber", "ID Lifetimecycle corrupted");
							}
						} else {
							// still waiting for another id
						}
					}
				}

				if (msg->message->getMethod() == Method::Create) {
					if (id != -1 && _existingObjects.find(id) == _existingObjects.end() && (waitForId == -1 || (_existingObjects.find(waitForId) != _existingObjects.end() && _existingObjects[waitForId] == IDStatus::CREATED))) {
						notifyNewID(id);
						notify = true;
					}
				}
			}
			_waitingMsgs[i].clear();
		}

		return notify;
	}

	void MessageSubscriber::notifyNewID(const int64_t id) {
		boost::mutex::scoped_lock sl(_objMessageListMutex);
		_newCreatedIDs.push_back(id);
	}

	void MessageSubscriber::deliverMessageInternal(const ReceivedMessagePtr & msg) {
#ifdef ISIXE_WITH_PROFILING
		msg->message->insertTimestamp("MessagingController deliverMessageInternal");
#endif /* ISIXE_WITH_PROFILING */
		// Step 5
		// Step 7
		// send messages to subscribers
		if (msg->message->getMethod() == Method::Delete) {
			// Delete Message updates id status
			_existingObjects[msg->message->getContent()->_id] = IDStatus::DELETED;

			// as the MessagingingSystems doesn't reorder messages after this function, we don't need to erase already delivered messages
			auto & waitingVector = _waitingMsgs[msg->message->getContent()->_id];
			std::vector<ReceivedMessagePtr> vNeu;
			// iterate over all messages
			for (size_t i = 0; i < waitingVector.size(); ++i) {
				// copy all Deletes to a tmp vector
				if (waitingVector[i]->message->getMethod() == Method::Delete) {
					vNeu.push_back(waitingVector[i]);
				}
			}
			_waitingMsgs[msg->message->getContent()->_id] = std::move(vNeu);
		}

		boost::mutex::scoped_lock objScopeLock(_objMessageVectorMutex);
		_objActiveMessageVector->push_back(msg);
	}

	void MessageSubscriber::reset() {
		boost::mutex::scoped_lock sl1(_objMessageVectorMutex);

		_objMessageVectorA.clear();
		_objMessageVectorB.clear();

		boost::mutex::scoped_lock sl2(_objMessageListMutex);
		_newCreatedIDs.clear();

		boost::mutex::scoped_lock sl3(_bufferLock);
		_existingObjects.clear();
		_waitingMsgs.clear();
	}

	void MessageSubscriber::processMessages() {
		while (updateBuffer());
		swapMessageBuffer();

		for (ReceivedMessagePtr & rm : *_objInActiveMessageVector) {
			Message::Ptr & m = rm->message;

			if (m->getSubtype() == 0) {
				reset();
				return;
			}
			if (m->getMessageType() == messages::MessageTypes::SubsystemMessageType) {
				continue;
			}
			assert(_ptrMessageMethod.find(m->getMessageType()) != _ptrMessageMethod.end());

#ifdef ISIXE_WITH_PROFILING
			m->getContent()->insertTimestamp("MessageSubscriber processMessages");
#endif /* ISIXE_WITH_PROFILING */

			_ptrMessageMethod[m->getMessageType()](m);
		}
		_objInActiveMessageVector->clear();
	}

} /* namespace core */
} /* namespace i6engine */
