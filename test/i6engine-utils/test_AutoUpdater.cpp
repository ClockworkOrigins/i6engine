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

#include <cstdint>

#include "i6engine/utils/AutoUpdater.h"

#include "gtest/gtest.h"

int32_t d = 0;

void testIt(int32_t i) {
	d = i;
}

TEST(AutoUpdater, operatorTest) {
	i6e::utils::AutoUpdater<int32_t> ud(5);

	int test = ud;

	EXPECT_EQ(5, test);

	ud = 50;
	test = ud;

	EXPECT_EQ(50, test);
}

TEST(AutoUpdater, Updater) {
	i6e::utils::AutoUpdater<int32_t> ud(5);

	ud.registerUpdate(std::bind(&testIt, std::placeholders::_1));

	ud = 50;

	EXPECT_EQ(50, d);

	ud = 22;

	EXPECT_EQ(22, d);

	ud = 222234242;

	EXPECT_EQ(222234242, d);

	ud = -353453533;

	EXPECT_EQ(-353453533, d);
}

TEST(AutoUpdater, operatorPlusPlus) {
	i6e::utils::AutoUpdater<int> i(5);

	EXPECT_EQ(5, i++);
	EXPECT_EQ(6, i);
	EXPECT_EQ(7, ++i);

	i6e::utils::AutoUpdater<uint64_t> ui(5);

	EXPECT_EQ(5, ui++);
	EXPECT_EQ(6, ui);
	EXPECT_EQ(7, ++ui);
}
