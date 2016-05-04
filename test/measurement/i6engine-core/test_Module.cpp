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

#define _GLIBCXX_USE_NANOSLEEP

#include <thread>

#include "i6engine/core/EngineCoreController.h"
#include "i6engine/core/messaging/MessagingController.h"
#include "i6engine/core/subsystem/ModuleController.h"
#include "i6engine/core/subsystem/SubSystemController.h"

#include "boost/make_shared.hpp"

#include "gtest/gtest.h"

typedef struct TestSubSystemMessage : public i6e::core::MessageStruct {
	uint32_t tick;
	TestSubSystemMessage(uint32_t t) : i6e::core::MessageStruct(), tick(t) {
	}
} TestSubSystemMessage;

class Test_SubSystem : public i6e::core::ModuleController {
public:
	Test_SubSystem(i6e::core::Subsystem subsystem) : i6e::core::ModuleController(subsystem), tickCounter() {
	}

	void OnThreadStart() {
	}

	void Tick() {
		tickCounter++;
	}

	void ShutDown() {
	}

	uint32_t tickCounter = 0;
};

class Test_SubSystem2 : public i6e::core::ModuleController {
public:
	Test_SubSystem2(i6e::core::Subsystem subsystem) : i6e::core::ModuleController(subsystem), tickCounter() {
	}

	void OnThreadStart() {
		_messagingController->registerMessageType(1, this);
		_ptrMessageMethod.insert(std::make_pair(1, [this](const i6e::core::Message::Ptr & m) {
			EXPECT_EQ(tickCounter, dynamic_cast<TestSubSystemMessage *>(m->getContent())->tick);
			receivedMessages++;
		}));
	}

	void Tick() {
		tickCounter++;
	}

	void ShutDown() {
		_messagingController->unregisterMessageType(1, this);
	}

	uint32_t tickCounter = 0;
	uint32_t receivedMessages = 0;
};

class Test_SubSystem3 : public i6e::core::ModuleController {
public:
	Test_SubSystem3(i6e::core::Subsystem subsystem) : i6e::core::ModuleController(subsystem), tickCounter() {
	}

	void OnThreadStart() {
	}

	void Tick() {
		for (size_t i = 0; i < 100; i++) {
			_messagingController->deliverMessage(boost::make_shared<i6e::core::Message>(1, 1, i6e::core::Method::Update, new TestSubSystemMessage(tickCounter), _subsystem));
		}
		tickCounter++;
	}

	void ShutDown() {
	}

	uint32_t tickCounter = 0;
};

TEST(Module, Ticking) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	Test_SubSystem * s1 = new Test_SubSystem(i6e::core::Subsystem::Graphic);
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, 100000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(975));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_EQ(10, s1->tickCounter);

	delete s1;
	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, TwoTicking) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	Test_SubSystem * s1 = new Test_SubSystem(i6e::core::Subsystem::Graphic);
	Test_SubSystem * s2 = new Test_SubSystem(i6e::core::Subsystem::Object);
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, 100000);
	s2->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s2, 50000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(975));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_EQ(10, s1->tickCounter);
	EXPECT_EQ(20, s2->tickCounter);

	delete s2;
	delete s1;
	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, Waiting) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	Test_SubSystem * s1 = new Test_SubSystem(i6e::core::Subsystem::Graphic);
	Test_SubSystem * s2 = new Test_SubSystem(i6e::core::Subsystem::Object);
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, { i6e::core::Subsystem::Object });
	s2->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s2, 100000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(975));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_EQ(10, s1->tickCounter);
	EXPECT_EQ(10, s2->tickCounter);

	delete s2;
	delete s1;
	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, WaitingForTwo) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	Test_SubSystem * s1 = new Test_SubSystem(i6e::core::Subsystem::Graphic);
	Test_SubSystem * s2 = new Test_SubSystem(i6e::core::Subsystem::Object);
	Test_SubSystem * s3 = new Test_SubSystem(i6e::core::Subsystem::Physic);
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, { i6e::core::Subsystem::Object, i6e::core::Subsystem::Physic });
	s2->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s2, 100000);
	s3->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s3, 50000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(990));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_EQ(10, s1->tickCounter);
	EXPECT_EQ(10, s2->tickCounter);
	EXPECT_EQ(20, s3->tickCounter);

	delete s3;
	delete s2;
	delete s1;
	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, WaitingChain) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	Test_SubSystem * s1 = new Test_SubSystem(i6e::core::Subsystem::Graphic);
	Test_SubSystem * s2 = new Test_SubSystem(i6e::core::Subsystem::Object);
	Test_SubSystem * s3 = new Test_SubSystem(i6e::core::Subsystem::Physic);
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, { i6e::core::Subsystem::Object });
	s2->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s2, { i6e::core::Subsystem::Physic });
	s3->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s3, 100000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(990));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_EQ(10, s1->tickCounter);
	EXPECT_EQ(10, s2->tickCounter);
	EXPECT_EQ(10, s3->tickCounter);

	delete s3;
	delete s2;
	delete s1;
	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, WaitingWithMessages) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	Test_SubSystem2 * s1 = new Test_SubSystem2(i6e::core::Subsystem::Graphic);
	Test_SubSystem3 * s2 = new Test_SubSystem3(i6e::core::Subsystem::Object);
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, { i6e::core::Subsystem::Object });
	s2->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s2, 10000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_GE(s1->tickCounter, 90U);
	EXPECT_GE(s2->tickCounter, 90U);
	EXPECT_LE(s1->tickCounter, 100U);
	EXPECT_LE(s2->tickCounter, 100U);
	EXPECT_GE(s1->receivedMessages, 900U);

	delete s2;
	delete s1;
	delete mc;
	delete ecc;
	delete ssc;
}
