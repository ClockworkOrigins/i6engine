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

#include <cmath>

#include "i6engine/utils/RealTimeClock.h"

#include "i6engine/core/clock/Scheduler.h"

#include "gtest/gtest.h"

class TestTimeClock {
public:
	explicit TestTimeClock(const boost::function<void(void)> & f) : _startTime(), _currentTime() {
	}

	~TestTimeClock() {
	}

	uint64_t getCurrentTime(uint64_t lastTime) {
		return _currentTime - _startTime;
	}

	void Stop() {
	}

	void passedTime(uint64_t timePassed) {
		_currentTime += timePassed;
	}

private:
	uint64_t _startTime;
	uint64_t _currentTime;
};

int func1_counter;

bool func1(uint64_t time, i6engine::utils::Clock<TestTimeClock> * cl, int counter) {
	EXPECT_LE(time, cl->getTime());
	EXPECT_LE(cl->getTime(), time + 3000);
	EXPECT_EQ(counter, func1_counter);
	func1_counter++;
	return true;
}

TEST(Scheduler, Once) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);
	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	func1_counter = 0;
	sched.runOnce(50000, boost::bind(func1, cl.getTime() + 50000, &cl, 0), 0);
	sched.runOnce(100000, boost::bind(func1, cl.getTime() + 100000, &cl, 3), 0);
	sched.runOnce(150000, boost::bind(func1, cl.getTime() + 150000, &cl, 4), 1);
	sched.runOnce(75000, boost::bind(func1, cl.getTime() + 75000, &cl, 2), 0);
	sched.runOnce(75000, boost::bind(func1, cl.getTime() + 75000, &cl, 1), 1);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	EXPECT_EQ(5, func1_counter);
}

std::vector<int> jobs;

bool func2(int counter) {
	jobs.push_back(counter);
	return true;
}

TEST(Scheduler, Repeat) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runRepeated(50050, boost::bind(func2, 0), 0);
	sched.runRepeated(100000, boost::bind(func2, 1), 0);
	sched.runRepeated(150000, boost::bind(func2, 2), 1);
	sched.runRepeated(75050, boost::bind(func2, 3), 0);
	sched.runRepeated(75050, boost::bind(func2, 4), 1);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(1));

    std::vector<int> ref({ 0, 4, 3, 1, 0, 2, 4, 3, 0, 1 });

	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, getTimeLeft) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	uint64_t id1 = sched.runOnce(500000, boost::bind(func2, 0), 0);
	uint64_t id2 = sched.runRepeated(50000, boost::bind(func2, 0), 0);

	for (int i = 0; i < 40; ++i) {
		cl.passedTime(5000);
		cl.Update();
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		EXPECT_LE(sched.getTimeLeft(id1), 500000 - 5000 * (i + 1)) << i;
		EXPECT_LE(sched.getTimeLeft(id2), 50000 - 5000 * ((i + 1) % 10)) << i;
	}

	EXPECT_EQ(UINT64_MAX, sched.getTimeLeft(3));
}

TEST(Scheduler, stop) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	uint64_t id1 = sched.runRepeated(50050, boost::bind(func2, 0), 0);
	sched.runRepeated(100000, boost::bind(func2, 1), 0);
	sched.runRepeated(150000, boost::bind(func2, 2), 1);
	uint64_t id4 = sched.runRepeated(75050, boost::bind(func2, 3), 0);
	sched.runRepeated(75050, boost::bind(func2, 4), 1);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
		if (i == 20) {
			while (!sched.stop(id1));
			while (!sched.stop(id4));
		}
	}

    std::vector<int> ref({ 0, 4, 3, 1, 2, 4 });
	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, removeTimerPriority) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runRepeated(50050, boost::bind(func2, 0), 0);
	sched.runRepeated(100000, boost::bind(func2, 1), 0);
	sched.runRepeated(150000, boost::bind(func2, 2), 1);
	sched.runRepeated(75050, boost::bind(func2, 3), 0);
	sched.runRepeated(75050, boost::bind(func2, 4), 1);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
		if (i == 20) {
			sched.removeTimer(0);
		}
	}

    std::vector<int> ref({ 0, 4, 3, 1, 2, 4 });
	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, addTimerLater) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runOnce(50000, boost::bind(func2, 0), 0);

	for (int i = 0; i < 6; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	sched.runOnce(15000, boost::bind(func2, 1), 0);

	for (int i = 0; i < 3; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	std::vector<int> ref({ 1 });
	EXPECT_EQ(ref, jobs);

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
	cl.passedTime(6000);
	cl.Update();

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

    std::vector<int> ref2({ 1, 0 });
	EXPECT_EQ(ref2, jobs);
}

void startClockAndScheduler() {
	i6engine::utils::Clock<i6engine::utils::RealTimeClock> cl;
	i6engine::core::Scheduler<i6engine::utils::RealTimeClock> sched(cl);

	boost::this_thread::sleep(boost::posix_time::milliseconds(10));

	sched.runOnce(5000, boost::bind(func2, 0), 0);
	sched.runRepeated(2500, boost::bind(func2, 1), 0);
	sched.runOnce(1500, boost::bind(func2, 1), 0);

	boost::this_thread::sleep(boost::posix_time::milliseconds(10));
}

TEST(Scheduler, startupAndShutdown) {
	for (uint32_t i = 0; i < 1000; i++) {
		startClockAndScheduler();
	}
}
