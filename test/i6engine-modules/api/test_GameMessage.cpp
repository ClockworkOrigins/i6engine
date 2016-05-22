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

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/ObjectConfig.h"

#include "boost/make_shared.hpp"

#include "gtest/gtest.h"

TEST(GameMessage, Serialize) {
	i6e::api::GameMessage::Ptr msg = boost::make_shared<i6e::api::GameMessage>(0, 0, i6e::core::Method::Update, new i6e::api::GameMessageStruct(), i6e::core::Subsystem::Unknown);
	msg->getContent()->_sender = i6e::core::IPKey("127.123.123.1:12345");

	EXPECT_FALSE(msg->Serialize().empty());
}

TEST(GameMessage, Position_Serialize) {
	i6e::api::GameMessage::Ptr msg1 = boost::make_shared<i6e::api::GameMessage>(i6e::api::messages::PositionMessageType, i6e::api::objects::Position, i6e::core::Method::Update, new i6e::api::objects::Position_Update(0, 0, Vec3(), Quaternion(), Vec3()), i6e::core::Subsystem::Unknown);
	i6e::api::GameMessage::Ptr msg2 = boost::make_shared<i6e::api::GameMessage>(i6e::api::messages::PositionMessageType, i6e::api::objects::Position, i6e::core::Method::Update, new i6e::api::objects::Position_Update(1000, 1001, Vec3(1000.0, 50.0, 1000.0), Quaternion(1.0, 0.0, 0.0, 0.0), Vec3(1.0, 1.0, 1.0)), i6e::core::Subsystem::Unknown);
	msg1->getContent()->_sender = i6e::core::IPKey("127.123.123.1:12345");
	msg2->getContent()->_sender = i6e::core::IPKey("127.123.123.1:12345");

	EXPECT_FALSE(msg1->Serialize().empty());
	EXPECT_FALSE(msg2->Serialize().empty());
}
