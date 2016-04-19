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

#include "i6engine/utils/Logger.h"

#include <thread>

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/MessageStruct.h"
#include "i6engine/core/messaging/MessageSubscriber.h"
#include "i6engine/core/messaging/MessagingController.h"

#include "boost/make_shared.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace i6engine::core;

class Mock_MessagingController : public MessagingController {
public:
	Mock_MessagingController() : MessagingController() {
	}
};

class Mock_SubSystem : public MessageSubscriber {
public:
	void Mailbox(const Message::Ptr &) const {
	}

	void processMessages() {
		while (MessageSubscriber::updateBuffer());
		MessageSubscriber::swapMessageBuffer();

		for (ReceivedMessagePtr & rm : *MessageSubscriber::_objInActiveMessageVector) {
			_receiveMessage(&(*rm->message));
		}
		MessageSubscriber::_objInActiveMessageVector->clear();
	}

	MOCK_METHOD1(_receiveMessage, void(Message *));

	Mock_MessagingController * _mc;
};

class Mock_SubSystemStress : public MessageSubscriber {
public:
	std::vector<int> status;

	Mock_SubSystemStress() : status(100 * 4), _mc(), stop(false), _condVariable(), _lock() {
	}

	void Mailbox(const Message::Ptr & msg) {
		if (msg->getSubtype() == 2) {
			stop = true;
			std::unique_lock<std::mutex> ul(_lock);
			_condVariable.notify_all();
		}
	}

	void processMessages() {
		while (MessageSubscriber::updateBuffer());
		MessageSubscriber::swapMessageBuffer();

		for (ReceivedMessagePtr & rm : *MessageSubscriber::_objInActiveMessageVector) {
			Mailbox(rm->message);
		}
		MessageSubscriber::_objInActiveMessageVector->clear();
	}

	Mock_MessagingController * _mc;
	bool stop;
	std::condition_variable _condVariable;
	std::mutex _lock;
};

TEST(MessagingController, Buffer1) {
	Mock_MessagingController * mc = new Mock_MessagingController();
	Mock_SubSystem ms;

	mc->registerMessageType(0, &ms);
	ms._mc = mc;

	// Update 1 Obj 10, waitfor 5
	Message::Ptr msg1(new Message(0, 1, Method::Create, new MessageStruct(-1, -1), i6engine::core::Subsystem::Unknown));
	// Update 1 Obj 10, waitfor 5
	Message::Ptr msg2(new Message(0, 1, Method::Create, new MessageStruct(-1, -1), i6engine::core::Subsystem::Unknown));

	 {
		::testing::InSequence dummy;

		EXPECT_CALL(ms, _receiveMessage(&(*msg1)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg2)));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(15));

	mc->deliverMessage(msg1);
	mc->deliverMessage(msg2);

	std::this_thread::sleep_for(std::chrono::milliseconds(15));

	ms.processMessages();

	mc->unregisterMessageType(0, &ms);

	delete mc;
}

TEST(MessagingController, Buffer2) {
	Mock_MessagingController * mc = new Mock_MessagingController();
	Mock_SubSystem ms;

	mc->registerMessageType(0, &ms);
	ms._mc = mc;

	// Update 1 Obj 10, waitfor 5
	Message::Ptr msg1(new Message(0, 1, Method::Update, new MessageStruct(10, 5), i6engine::core::Subsystem::Unknown));
	// Update 2 Obj 10, waitfor 5
	Message::Ptr msg2(new Message(0, 1, Method::Update, new MessageStruct(10, 5), i6engine::core::Subsystem::Unknown));
	// Create Obj 10, waitfor 5
	Message::Ptr msg3(new Message(0, 1, Method::Create, new MessageStruct(10, 5), i6engine::core::Subsystem::Unknown));
	// Create Obj 5
	Message::Ptr msg4(new Message(0, 1, Method::Create, new MessageStruct(5, -1), i6engine::core::Subsystem::Unknown));
	// Delete Obj 10
	Message::Ptr msg5(new Message(0, 1, Method::Delete, new MessageStruct(10, -1), i6engine::core::Subsystem::Unknown));
	// Update 3 Obj 10, waitfor 5
	Message::Ptr msg6(new Message(0, 1, Method::Update, new MessageStruct(10, 5), i6engine::core::Subsystem::Unknown));

	 {
		::testing::InSequence dummy;

		EXPECT_CALL(ms, _receiveMessage(&(*msg4)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg3)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg1)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg2)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg5)));
		// Message 6 will never be delivered because of Message 5 arrived earlier
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(15));

	mc->deliverMessage(msg1);
	mc->deliverMessage(msg2);
	mc->deliverMessage(msg3);
	mc->deliverMessage(msg4);
	mc->deliverMessage(msg5);
	mc->deliverMessage(msg6);

	std::this_thread::sleep_for(std::chrono::milliseconds(15));

	ms.processMessages();

	mc->unregisterMessageType(0, &ms);

	delete mc;
}

TEST(MessagingController, specialCases) {
	Mock_MessagingController * mc = new Mock_MessagingController();
	Mock_SubSystem ms;

	mc->registerMessageType(0, &ms);
	ms._mc = mc;

	// create and delete obj 1
	Message::Ptr msg1(new Message(0, 0, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg2(new Message(0, 0, Method::Delete, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	// create and delete obj 2
	Message::Ptr msg3(new Message(0, 0, Method::Create, new MessageStruct(2, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg4(new Message(0, 0, Method::Delete, new MessageStruct(2, -1), i6engine::core::Subsystem::Unknown));
	// This message mustn't be delivered because parent (2) is already deleted
	Message::Ptr msg5(new Message(0, 0, Method::Delete, new MessageStruct(1, 2), i6engine::core::Subsystem::Unknown));

	{
		::testing::InSequence dummy;

		EXPECT_CALL(ms, _receiveMessage(&(*msg1)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg3)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg2)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg4)));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	mc->deliverMessage(msg1);
	mc->deliverMessage(msg2);
	mc->deliverMessage(msg3);
	mc->deliverMessage(msg4);

	// wait for messages to be delivered and processed
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ms.processMessages();

	// now comes a delete
	mc->deliverMessage(msg5);

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	ms.processMessages();

	mc->unregisterMessageType(0, &ms);

	delete mc;
}

TEST(MessagingController, updateAfterDelete) {
	Mock_MessagingController * mc = new Mock_MessagingController();
	Mock_SubSystem ms;

	mc->registerMessageType(0, &ms);
	ms._mc = mc;

	Message::Ptr msg1(new Message(0, 0, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg2(new Message(0, 0, Method::Create, new MessageStruct(2, 1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg3(new Message(0, 0, Method::Update, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg4(new Message(0, 0, Method::Update, new MessageStruct(2, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg5(new Message(0, 0, Method::Delete, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg6(new Message(0, 0, Method::Delete, new MessageStruct(2, 1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg7(new Message(0, 0, Method::Update, new MessageStruct(2, -1), i6engine::core::Subsystem::Unknown));

	{
		::testing::InSequence dummy;

		EXPECT_CALL(ms, _receiveMessage(&(*msg1)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg2)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg3)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg4)));
		EXPECT_CALL(ms, _receiveMessage(&(*msg5)));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	mc->deliverMessage(msg1);
	mc->deliverMessage(msg2);
	mc->deliverMessage(msg3);
	mc->deliverMessage(msg4);

	// wait for messages to be delivered and processed
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ms.processMessages();

	// now comes a delete
	mc->deliverMessage(msg5);
	mc->deliverMessage(msg6);
	mc->deliverMessage(msg7);

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	ms.processMessages();

	mc->unregisterMessageType(0, &ms);

	delete mc;
}

TEST(MessagingController, StressTest) {
	const int numObjs = 100; // modify in Mock_SubSystemStress as well!

	Mock_MessagingController * mc = new Mock_MessagingController();
	Mock_SubSystemStress ms;

	mc->registerMessageType(0, &ms);
	ms._mc = mc;

	std::vector<Method> methods = { Method::Create, Method::Update, Method::Delete };
	std::vector<Message::Ptr> msgs;
	// create a bunch of create messages
	// * numObjs basic msgs
	// * 2*numObjs objs depending on the first
	// * numObjs objs waiting on the second
	msgs.resize(808 * numObjs);
	size_t msgCounter = 0;
	for (int i = 0; i < numObjs; i++) {
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(i, -1), i6engine::core::Subsystem::Unknown);
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(i + numObjs, i), i6engine::core::Subsystem::Unknown);
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(i + 2 * numObjs, i), i6engine::core::Subsystem::Unknown);
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(i + 3 * numObjs, i + numObjs), i6engine::core::Subsystem::Unknown);
	}
	// create update messages
	for (int i = 0; i < numObjs; i++) {
		for (int j = 0; j < 100; j++) {
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(i, -1), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(-1, i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(i, i + 1), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(i, 2 * i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(i, 3 * i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(i + 1, i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(2 * i, 3 * i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(3 * i, i), i6engine::core::Subsystem::Unknown);
		}
	}
	// delete messages
	for (int i = 0; i < 4 * numObjs; i++) {
		msgs[msgCounter++] = boost::make_shared<Message>(0, 0, Method::Delete, new MessageStruct(i, -1), i6engine::core::Subsystem::Unknown);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	srand(0);
	std::vector<std::pair<int, size_t>> allMsgs(msgCounter);
	for (size_t i = 0; i < msgCounter; i++) {
		allMsgs[i].first = rand();
		allMsgs[i].second = i;
	}
	std::sort(allMsgs.begin(), allMsgs.end());

	for (size_t i = 0; i < msgCounter; i++) {
		mc->deliverMessage(msgs[allMsgs[i].second]);
	}

	for (int i = 0; i < 100; i++) {
		ms.processMessages();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	mc->unregisterMessageType(0, &ms);

	delete mc;
}

void bombMessages(int numObjs, Mock_MessagingController * mc, int id) {
	std::vector<Message::Ptr> msgs;
	// create a bunch of create messages
	// * numObjs basic msgs
	// * 2*numObjs objs depending on the first
	// * numObjs objs waiting on the second
	msgs.resize(size_t(808 * numObjs));
	size_t msgCounter = 0;
	for (int i = 0; i < numObjs; i++) {
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(id + i, -1), i6engine::core::Subsystem::Unknown);
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(id + i + numObjs, id + i), i6engine::core::Subsystem::Unknown);
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(id + i + 2 * numObjs, id + i), i6engine::core::Subsystem::Unknown);
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Create, new MessageStruct(id + i + 3 * numObjs, id + i + numObjs), i6engine::core::Subsystem::Unknown);
	}
	// create update messages
	for (int i = 0; i < numObjs; i++) {
		for (int j = 0; j < 100; j++) {
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + i, -1), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(-1, id + i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + i, id + i + 1), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + i, id + 2 * i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + i, id + 3 * i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + i + 1, id + i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + 2 * i, id + 3 * i), i6engine::core::Subsystem::Unknown);
			msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Update, new MessageStruct(id + 3 * i, id + i), i6engine::core::Subsystem::Unknown);
		}
	}
	// delete messages
	for (int i = 0; i < 4 * numObjs; i++) {
		msgs[msgCounter++] = boost::make_shared<Message>(0, 1, Method::Delete, new MessageStruct(id + i, -1), i6engine::core::Subsystem::Unknown);
	}

	srand(0);
	std::vector<std::pair<int, size_t>> allMsgs(msgCounter);
	for (size_t i = 0; i < msgCounter; i++) {
		allMsgs[i].first = rand();
		allMsgs[i].second = i;
	}
	std::sort(allMsgs.begin(), allMsgs.end());

	for (size_t i = 0; i < msgCounter; i++) {
		mc->deliverMessage(msgs[allMsgs[i].second]);
	}
}

TEST(MessagingController, MultithreadedStressTest) {
	const int numObjs = 100; // modify in Mock_SubSystemStress as well!

	Mock_MessagingController * mc = new Mock_MessagingController();
	Mock_SubSystemStress ms;

	mc->registerMessageType(0, &ms);
	ms._mc = mc;

	std::vector<Method> methods = { Method::Create, Method::Update, Method::Delete };

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	std::thread([&ms](){
		while (!ms.stop) {
			ms.processMessages();
		}
	}).detach();

	std::vector<std::thread *> threads;

	const unsigned int THREADS = 10;

	for (unsigned int i = 0; i < THREADS; i++) {
		threads.push_back(new std::thread(bombMessages, numObjs, mc, THREADS * i));
	}

	for (unsigned int i = 0; i < THREADS; i++) {
		threads[i]->join();
		delete threads[i];
	}

	mc->deliverMessage(boost::make_shared<Message>(0, 2, Method::Update, new MessageStruct(), i6engine::core::Subsystem::Unknown));

	std::unique_lock<std::mutex> ul(ms._lock);
	ms._condVariable.wait(ul);

	mc->unregisterMessageType(0, &ms);

	delete mc;
}

TEST(MessagingController, handlingBufferedMessageForUnregisteredType) {
	Mock_MessagingController * mc = new Mock_MessagingController();
	MessageSubscriber ms;

	mc->registerMessageType(1, &ms);
	ms.addMethod(1, [](const Message::Ptr) {});

	// create and delete obj 1
	Message::Ptr msg1(new Message(1, 1, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg2(new Message(1, 1, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg3(new Message(1, 1, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg4(new Message(1, 1, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg5(new Message(1, 1, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));
	Message::Ptr msg6(new Message(1, 1, Method::Create, new MessageStruct(1, -1), i6engine::core::Subsystem::Unknown));

	mc->deliverMessage(msg1);
	mc->deliverMessage(msg2);
	mc->deliverMessage(msg3);

	// wait for messages to be delivered and processed
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ms.processMessages();

	// this message will be in buffer when unregistering
	mc->deliverMessage(msg4);

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	mc->unregisterMessageType(1, &ms);
	ms.removeMethod(1);
	mc->deliverMessage(msg5);

	ms.processMessages();

	delete mc;
}
