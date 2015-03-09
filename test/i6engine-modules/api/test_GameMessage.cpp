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
	i6engine::api::GameMessage::Ptr msg = boost::make_shared<i6engine::api::GameMessage>(0, 0, i6engine::core::Method::Update, new i6engine::api::GameMessageStruct(), i6engine::core::Subsystem::Unknown);

	EXPECT_LT(msg->Serialize().size(), 30);
}

TEST(GameMessage, Position_Serialize) {
	i6engine::api::GameMessage::Ptr msg1 = boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::PositionMessageType, i6engine::api::objects::Position, i6engine::core::Method::Update, new i6engine::api::objects::Position_Update(0, 0, Vec3(), Quaternion(), Vec3()), i6engine::core::Subsystem::Unknown);
	i6engine::api::GameMessage::Ptr msg2 = boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::PositionMessageType, i6engine::api::objects::Position, i6engine::core::Method::Update, new i6engine::api::objects::Position_Update(1000, 1001, Vec3(1000.0, 50.0, 1000.0), Quaternion(1.0, 0.0, 0.0, 0.0), Vec3(1.0, 1.0, 1.0)), i6engine::core::Subsystem::Unknown);

	std::cout << msg1->Serialize().size() << std::endl;
	std::cout << msg1->Serialize() << std::endl;

	EXPECT_LT(msg1->Serialize().size(), 50);

	std::cout << "-----------------------------" << std::endl;
	std::cout << msg2->Serialize().size() << std::endl;
	std::cout << msg2->Serialize() << std::endl;

	EXPECT_LT(msg2->Serialize().size(), 50);
}
