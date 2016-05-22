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

#include <chrono>
#include <thread>

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#define __STDC_LIMIT_MACROS
	#include <stdint.h>
#endif

#include "i6engine/i6engineBuildSettings.h"

#include "i6engine/utils/Clock.h"

#include "boost/function.hpp"

#include "gtest/gtest.h"

class TestTimeClock {
public:
	explicit TestTimeClock(const boost::function<void(void)> &) : _startTime(), _currentTime() {
	}

	~TestTimeClock() {
	}

	uint64_t getCurrentTime(uint64_t) {
		return _currentTime - _startTime;
	}

	void Stop() {
	}

	void passedTime(uint64_t timePassed) {
		_currentTime += timePassed;
	}

protected:
	void Init() {
	}

private:
	uint64_t _startTime;
	uint64_t _currentTime;
};

TEST(Clock, RealTime) {
	i6e::utils::Clock<TestTimeClock> clock;

	uint64_t lastTime = clock.getTime();
	lastTime += 2000;
	for (int i = 1; i < 10; ++i) {
		clock.passedTime(50000);
		clock.Update();
		// set the value on the right to anything that will work. As long it is << 50.000
		// the test is ok. It's just .. this loop takes more than 0 microsecssecs
		uint64_t a = clock.getTime();
		EXPECT_LT(a - lastTime, 51000);
		lastTime = a;
	}

	EXPECT_EQ(0, clock.registerTimer());
	EXPECT_EQ(1, clock.registerTimer());
	EXPECT_EQ(2, clock.registerTimer());
}

uint64_t sT;
bool done;

void func(i6e::utils::Clock<TestTimeClock> * c) {
	for (int i = 0; i < 10; ++i) {
		// wait for some time
		uint64_t wT = 50000;
		c->waitForTime(0, sT + wT);
		uint64_t t = c->getTime();
		EXPECT_LE(sT + wT, t);
		sT = t;
	}
	done = true;
}

TEST(Clock, Notifies) {
    i6e::utils::Clock<TestTimeClock> * clock = new i6e::utils::Clock<TestTimeClock>();
	uint64_t ind = clock->registerTimer();
    EXPECT_EQ(ind, 0);
	sT = clock->getTime();
    done = false;
	std::thread(std::bind(func, clock)).detach();
    for (int i = 0; i < 20; ++i) {
    	std::this_thread::sleep_for(std::chrono::milliseconds(5));
    	clock->passedTime(20000);
    	clock->Update();
	}
    EXPECT_FALSE(done);
    for (int i = 0; i < 30; ++i) {
    	std::this_thread::sleep_for(std::chrono::milliseconds(5));
    	clock->passedTime(20000);
    	clock->Update();
	}
    EXPECT_TRUE(done);
	delete clock;
}
