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

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/ObjectConfig.h"

#include "boost/make_shared.hpp"

#include "gtest/gtest.h"

TEST(GameMessage, Serialize) {
	i6e::api::GameMessage::Ptr msg = boost::make_shared<i6e::api::GameMessage>(0, 0, i6e::core::Method::Update, new i6e::api::GameMessageStruct(), i6e::core::Subsystem::Unknown);
	msg->getContent()->_sender = i6e::core::IPKey("127.123.123.1:12345");

	EXPECT_LT(msg->Serialize().size(), 30U);
}

TEST(GameMessage, Position_Serialize) {
	i6e::api::GameMessage::Ptr msg1 = boost::make_shared<i6e::api::GameMessage>(i6e::api::messages::PositionMessageType, i6e::api::objects::Position, i6e::core::Method::Update, new i6e::api::objects::Position_Update(0, 0, Vec3(), Quaternion(), Vec3()), i6e::core::Subsystem::Unknown);
	i6e::api::GameMessage::Ptr msg2 = boost::make_shared<i6e::api::GameMessage>(i6e::api::messages::PositionMessageType, i6e::api::objects::Position, i6e::core::Method::Update, new i6e::api::objects::Position_Update(1000, 1001, Vec3(1000.0, 50.0, 1000.0), Quaternion(1.0, 0.0, 0.0, 0.0), Vec3(1.0, 1.0, 1.0)), i6e::core::Subsystem::Unknown);
	msg1->getContent()->_sender = i6e::core::IPKey("127.123.123.1:12345");
	msg2->getContent()->_sender = i6e::core::IPKey("127.123.123.1:12345");

	std::cout << msg1->Serialize().size() << std::endl;
	std::cout << msg1->Serialize() << std::endl;

	EXPECT_LT(msg1->Serialize().size(), 50U);

	std::cout << "-----------------------------" << std::endl;
	std::cout << msg2->Serialize().size() << std::endl;
	std::cout << msg2->Serialize() << std::endl;

	EXPECT_LT(msg2->Serialize().size(), 50U);
}
