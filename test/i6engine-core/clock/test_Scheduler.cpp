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

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#define __STDC_LIMIT_MACROS
	#include <stdint.h>
#endif

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

protected:
	void Init() {
	}

private:
	uint64_t _startTime;
	uint64_t _currentTime;
};

int func1_counter;
std::mutex jobLock;

bool func1(uint64_t time, i6engine::utils::Clock<TestTimeClock> * cl, int counter) {
	std::lock_guard<std::mutex> lg(jobLock);
	EXPECT_LE(time, cl->getTime());
	EXPECT_EQ(counter, func1_counter);
	func1_counter++;
	return true;
}

TEST(Scheduler, Once) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);
	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	func1_counter = 0;
	sched.runOnce(50001, boost::bind(func1, cl.getTime() + 50001, &cl, 0), i6engine::core::JobPriorities::Prio_Low);
	sched.runOnce(100000, boost::bind(func1, cl.getTime() + 100000, &cl, 2), i6engine::core::JobPriorities::Prio_Low);
	sched.runOnce(150000, boost::bind(func1, cl.getTime() + 150000, &cl, 3), i6engine::core::JobPriorities::Prio_Medium);
	sched.runOnce(75001, boost::bind(func1, cl.getTime() + 75001, &cl, 1), i6engine::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	EXPECT_EQ(4, func1_counter);
}

std::vector<int> jobs;

bool func2(int counter) {
	std::lock_guard<std::mutex> lg(jobLock);
	jobs.push_back(counter);
	return true;
}

TEST(Scheduler, Repeat) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runRepeated(50001, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(100000, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(150000, boost::bind(func2, 2), i6engine::core::JobPriorities::Prio_Medium);
	sched.runRepeated(75001, boost::bind(func2, 3), i6engine::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(1));

	std::vector<int> ref({ 0, 3, 1, 0, 2, 3, 0, 1 });

	std::lock_guard<std::mutex> lg(jobLock);
	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, getTimeLeft) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	uint64_t id1 = sched.runOnce(500000, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);
	uint64_t id2 = sched.runRepeated(50000, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);

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

	{
		std::lock_guard<std::mutex> lg(jobLock);
		jobs.clear();
	}

	uint64_t id1 = sched.runRepeated(50001, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(100000, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(150000, boost::bind(func2, 2), i6engine::core::JobPriorities::Prio_Medium);
	uint64_t id4 = sched.runRepeated(75001, boost::bind(func2, 3), i6engine::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
		if (i == 20) {
			while (!sched.stop(id1));
			while (!sched.stop(id4));
		}
	}

	std::vector<int> ref({ 0, 3, 1, 2, 1 });

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
	std::lock_guard<std::mutex> lg(jobLock);
	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, removeTimerPriority) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runRepeated(50001, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(100000, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(150000, boost::bind(func2, 2), i6engine::core::JobPriorities::Prio_Medium);
	sched.runRepeated(75001, boost::bind(func2, 3), i6engine::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
		if (i == 20) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
			sched.removeTimer(i6engine::core::JobPriorities::Prio_Low);
		}
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    std::vector<int> ref({ 0, 3, 1, 2 });
	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, addTimerLater) {
	i6engine::utils::Clock<TestTimeClock> cl;
	i6engine::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runOnce(50000, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 6; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	sched.runOnce(15000, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);

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

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	sched.runOnce(5000, boost::bind(func2, 0), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runOnce(1500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(3000, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(4500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);
	sched.runRepeated(3500, boost::bind(func2, 1), i6engine::core::JobPriorities::Prio_Low);

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
}

TEST(Scheduler, startupAndShutdown) {
	for (uint32_t i = 0; i < 100; i++) {
		startClockAndScheduler();
	}
}
