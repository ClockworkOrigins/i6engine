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

#include "i6engine/utils/RealTimeClock.h"

#include "gtest/gtest.h"

void emptyMethod() {}

TEST(RealTimeClock, getCurrentTime) {
	i6e::utils::RealTimeClock clock(boost::bind(&emptyMethod));

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
	i6e::utils::RealTimeClock clock(boost::bind(&emptyMethod));

	uint64_t firstTime = clock.getCurrentTime(0);
	uint64_t secondTime = clock.getCurrentTime(firstTime);

	EXPECT_LE(firstTime, secondTime);

	clock.setCurrentTime(1000);

	firstTime = clock.getCurrentTime(secondTime);

	EXPECT_LE(secondTime, firstTime);
	EXPECT_LE(1000, firstTime);
}
