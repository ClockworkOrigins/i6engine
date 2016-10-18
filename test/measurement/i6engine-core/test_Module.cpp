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
	std::shared_ptr<Test_SubSystem> s1 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Graphic));
	s1->setController(ssc, ecc, mc);
	ssc->QueueSubSystemStart(s1, 100000);

	std::thread([ecc]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(975));

		ecc->ShutDown();
	}).detach();

	ecc->RunEngine();

	EXPECT_EQ(10, s1->tickCounter);

	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, TwoTicking) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	std::shared_ptr<Test_SubSystem> s1 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Graphic));
	std::shared_ptr<Test_SubSystem> s2 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Object));
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

	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, Waiting) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	std::shared_ptr<Test_SubSystem> s1 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Graphic));
	std::shared_ptr<Test_SubSystem> s2 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Object));
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

	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, WaitingForTwo) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	std::shared_ptr<Test_SubSystem> s1 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Graphic));
	std::shared_ptr<Test_SubSystem> s2 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Object));
	std::shared_ptr<Test_SubSystem> s3 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Physic));
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

	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, WaitingChain) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	std::shared_ptr<Test_SubSystem> s1 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Graphic));
	std::shared_ptr<Test_SubSystem> s2 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Object));
	std::shared_ptr<Test_SubSystem> s3 = std::shared_ptr<Test_SubSystem>(new Test_SubSystem(i6e::core::Subsystem::Physic));
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

	delete mc;
	delete ecc;
	delete ssc;
}

TEST(Module, WaitingWithMessages) {
	i6e::core::SubSystemController * ssc = new i6e::core::SubSystemController();
	i6e::core::EngineCoreController * ecc = new i6e::core::EngineCoreController(ssc);
	i6e::core::MessagingController * mc = new i6e::core::MessagingController();
	ssc->registerController(ecc);
	std::shared_ptr<Test_SubSystem2> s1 = std::shared_ptr<Test_SubSystem2>(new Test_SubSystem2(i6e::core::Subsystem::Graphic));
	std::shared_ptr<Test_SubSystem3> s2 = std::shared_ptr<Test_SubSystem3>(new Test_SubSystem3(i6e::core::Subsystem::Object));
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

	delete mc;
	delete ecc;
	delete ssc;
}
