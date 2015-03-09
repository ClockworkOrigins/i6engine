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

#include "i6engine/utils/AutoUpdater.h"

#include "gtest/gtest.h"

int d = 0;

void testIt(std::string s, int i) {
	d = i;
}

TEST(AutoUpdater, operatorTest) {
	i6engine::utils::AutoUpdater<int> ud(5);

	int test = ud;

	EXPECT_EQ(5, test);

	ud = 50;
	test = ud;

	EXPECT_EQ(50, test);
}

TEST(AutoUpdater, Updater) {
	i6engine::utils::AutoUpdater<int> ud(5);

	std::function<void(std::string, int)> f = std::bind(&testIt, std::placeholders::_1, std::placeholders::_2);

	ud.registerUpdate(std::string("TEST"), std::bind(&testIt, std::placeholders::_1, std::placeholders::_2));

	ud = 50;

	EXPECT_EQ(50, d);

	ud = 22;

	EXPECT_EQ(22, d);

	ud = 222234242;

	EXPECT_EQ(222234242, d);

	ud = -353453533;

	EXPECT_EQ(-353453533, d);
}
