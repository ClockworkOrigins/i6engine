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

#include "i6engine/utils/RealTimeClock.h"

#include "gtest/gtest.h"

void emptyMethod() {}

TEST(RealTimeClock, getCurrentTime) {
	i6e::utils::RealTimeClock clock(std::bind(&emptyMethod));
	clock.Init();

	uint64_t firstTime = clock.getCurrentTime(0);
	uint64_t secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);

	secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);
}

TEST(RealTimeClock, setCurrentTime) {
	i6e::utils::RealTimeClock clock(std::bind(&emptyMethod));
	clock.Init();

	uint64_t firstTime = clock.getCurrentTime(0);
	uint64_t secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	clock.setCurrentTime(secondTime + 1000);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);
	EXPECT_LE(secondTime + 1000, firstTime);
}
