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

#include <boost/shared_ptr.hpp>

#include "i6engine/core/configs/SubsystemConfig.h"
#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/messaging/MessageStruct.h"

#include "gtest/gtest.h"

TEST(Message, Constructor) {
	i6engine::core::IPKey sender;

	i6engine::core::Message::Ptr msg1(new i6engine::core::Message(0, 0, i6engine::core::Method::Create, new i6engine::core::MessageStruct(-1, sender, 15), i6engine::core::Subsystem::Unknown));

	EXPECT_EQ(0, msg1->getMessageType());
	EXPECT_EQ(0, msg1->getSubtype());
	EXPECT_EQ(i6engine::core::Method::Create, msg1->getMethod());
	EXPECT_EQ(-1, msg1->getContent()->getID());
	EXPECT_EQ(15, msg1->getContent()->getWaitID());
	EXPECT_EQ(sender, msg1->getContent()->_sender);

	i6engine::core::Message::Ptr msg2(new i6engine::core::Message(*msg1));

	EXPECT_EQ(0, msg2->getMessageType());
	EXPECT_EQ(0, msg2->getSubtype());
	EXPECT_EQ(i6engine::core::Method::Create, msg2->getMethod());
	EXPECT_EQ(-1, msg2->getContent()->getID());
	EXPECT_EQ(15, msg2->getContent()->getWaitID());
	EXPECT_EQ(sender, msg2->getContent()->_sender);
	EXPECT_NE(msg1->getContent(), msg2->getContent());

	msg1->getContent()->_id = 5;

	EXPECT_NE(msg1->getContent()->getID(), msg2->getContent()->getID());
}

TEST(Message, setMessageType) {
	i6engine::core::Message::Ptr msg(new i6engine::core::Message(0, 0, i6engine::core::Method::Create, new i6engine::core::MessageStruct(-1, 15), i6engine::core::Subsystem::Unknown));

	EXPECT_EQ(0, msg->getMessageType());

	msg->setMessageType(1);

	EXPECT_EQ(1, msg->getMessageType());
}

TEST(Message, Serialize) {
	i6engine::core::Message::Ptr msg(new i6engine::core::Message(0, 0, i6engine::core::Method::Create, new i6engine::core::MessageStruct(), i6engine::core::Subsystem::Unknown));

	std::cout << msg->Serialize().size() << std::endl;
	std::cout << msg->Serialize() << std::endl;

	EXPECT_LT(msg->Serialize().size(), 20);
}
