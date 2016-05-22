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

#define _GLIBCXX_USE_NANOSLEEP

#include <thread>

#include "i6engine/i6engineBuildSettings.h"

#include "i6engine/utils/Singleton.h"

#include "gtest/gtest.h"

class SingletonTest : public i6e::utils::Singleton<SingletonTest> {
public:
	SingletonTest() {
		_counter++;
	}

	int getCounter() const {
		return _counter;
	}

private:
	static int _counter;
};

int SingletonTest::_counter = 0;

TEST(Singleton, Init) {
	for (unsigned int i = 0; i < 1000; i++) {
		std::thread([](){ SingletonTest::GetSingleton(); }).detach();
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));

	EXPECT_EQ(1, SingletonTest::GetSingleton().getCounter());
	EXPECT_EQ(1, SingletonTest::GetSingletonPtr()->getCounter());

	SingletonTest * ptr = SingletonTest::GetSingletonPtr();

	for (unsigned int i = 0; i < 1000; i++) {
		EXPECT_EQ(ptr, SingletonTest::GetSingletonPtr());
	}
}
