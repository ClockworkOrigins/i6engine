/*
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
#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/messaging/MessageStruct.h"

#include "boost/shared_ptr.hpp"

#include "gtest/gtest.h"

TEST(Message, Serialize) {
	i6e::core::Message::Ptr msg(new i6e::core::Message(0, 0, i6e::core::Method::Create, new i6e::core::MessageStruct(), i6e::core::Subsystem::Unknown));

	std::cout << msg->Serialize().size() << std::endl;
	std::cout << msg->Serialize() << std::endl;

	EXPECT_LT(msg->Serialize().size(), 20u);
}
