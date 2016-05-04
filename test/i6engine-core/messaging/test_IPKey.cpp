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

#include "i6engine/core/messaging/IPKey.h"

#include "gtest/gtest.h"

using i6e::core::IPKey;

TEST(IPKey, Equality) {
	IPKey k1("127.0.0.1", 4711);
	IPKey k2("127.0.0.1", 4711);
	IPKey k3("192.168.1.1", 4711);
	IPKey k4("127.0.0.1", 1337);
	IPKey k5, k6;

	EXPECT_EQ(k1, k2);
	EXPECT_EQ(k5, k6);

	EXPECT_NE(k1, k3);
	EXPECT_NE(k1, k4);
	EXPECT_NE(k3, k4);
	EXPECT_NE(k1, k5);
}

TEST(IPKey, isValid) {
	IPKey valid("127.0.0.1", 4711);
	IPKey invalid;

	EXPECT_TRUE(valid.isValid());
	EXPECT_FALSE(invalid.isValid());
}
