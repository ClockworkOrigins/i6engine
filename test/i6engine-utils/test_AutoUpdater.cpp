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
