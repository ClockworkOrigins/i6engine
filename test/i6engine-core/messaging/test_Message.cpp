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

#include "i6engine/core/configs/SubsystemConfig.h"
#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/messaging/MessageStruct.h"

#include "boost/shared_ptr.hpp"

#include "gtest/gtest.h"

TEST(Message, Constructor) {
	i6e::core::IPKey sender;

	i6e::core::Message::Ptr msg1(new i6e::core::Message(0, 0, i6e::core::Method::Create, new i6e::core::MessageStruct(-1, sender, 15), i6e::core::Subsystem::Unknown));

	EXPECT_EQ(0, msg1->getMessageType());
	EXPECT_EQ(0, msg1->getSubtype());
	EXPECT_EQ(i6e::core::Method::Create, msg1->getMethod());
	EXPECT_EQ(-1, msg1->getContent()->getID());
	EXPECT_EQ(15, msg1->getContent()->getWaitID());
	EXPECT_EQ(sender, msg1->getContent()->_sender);

	i6e::core::Message::Ptr msg2(new i6e::core::Message(*msg1));

	EXPECT_EQ(0, msg2->getMessageType());
	EXPECT_EQ(0, msg2->getSubtype());
	EXPECT_EQ(i6e::core::Method::Create, msg2->getMethod());
	EXPECT_EQ(-1, msg2->getContent()->getID());
	EXPECT_EQ(15, msg2->getContent()->getWaitID());
	EXPECT_EQ(sender, msg2->getContent()->_sender);
	EXPECT_NE(msg1->getContent(), msg2->getContent());

	msg1->getContent()->_id = 5;

	EXPECT_NE(msg1->getContent()->getID(), msg2->getContent()->getID());
}

TEST(Message, setMessageType) {
	i6e::core::Message::Ptr msg(new i6e::core::Message(0, 0, i6e::core::Method::Create, new i6e::core::MessageStruct(-1, 15), i6e::core::Subsystem::Unknown));

	EXPECT_EQ(0, msg->getMessageType());

	msg->setMessageType(1);

	EXPECT_EQ(1, msg->getMessageType());
}

TEST(Message, Serialize) {
	i6e::core::Message::Ptr msg(new i6e::core::Message(0, 0, i6e::core::Method::Create, new i6e::core::MessageStruct(), i6e::core::Subsystem::Unknown));

	msg->Serialize();

	EXPECT_FALSE(msg->Serialize().empty());
}
