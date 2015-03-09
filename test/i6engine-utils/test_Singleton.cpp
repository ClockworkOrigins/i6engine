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

#define _GLIBCXX_USE_NANOSLEEP

#include <thread>

#include "i6engine/i6engineBuildSettings.h"

#include "i6engine/utils/Singleton.h"

#include "gtest/gtest.h"

class SingletonTest : public i6engine::utils::Singleton<SingletonTest> {
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
