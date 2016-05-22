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

int func1_counter;
std::mutex jobLock;

bool func1(uint64_t time, i6e::utils::Clock<TestTimeClock> * cl, int counter) {
	std::lock_guard<std::mutex> lg(jobLock);
	EXPECT_LE(time, cl->getTime());
	EXPECT_EQ(counter, func1_counter);
	func1_counter++;
	return true;
}

TEST(Scheduler, Once) {
	i6e::utils::Clock<TestTimeClock> cl;
	i6e::core::Scheduler<TestTimeClock> sched(cl);
	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	func1_counter = 0;
	sched.runOnce(50001, boost::bind(func1, cl.getTime() + 50001, &cl, 0), i6e::core::JobPriorities::Prio_Low);
	sched.runOnce(100000, boost::bind(func1, cl.getTime() + 100000, &cl, 2), i6e::core::JobPriorities::Prio_Low);
	sched.runOnce(150000, boost::bind(func1, cl.getTime() + 150000, &cl, 3), i6e::core::JobPriorities::Prio_Medium);
	sched.runOnce(75001, boost::bind(func1, cl.getTime() + 75001, &cl, 1), i6e::core::JobPriorities::Prio_Low);

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
	i6e::utils::Clock<TestTimeClock> cl;
	i6e::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runRepeated(50001, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(100000, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(150000, boost::bind(func2, 2), i6e::core::JobPriorities::Prio_Medium);
	sched.runRepeated(75001, boost::bind(func2, 3), i6e::core::JobPriorities::Prio_Low);

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
	i6e::utils::Clock<TestTimeClock> cl;
	i6e::core::Scheduler<TestTimeClock> sched(cl);

	uint64_t id1 = sched.runOnce(500000, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);
	uint64_t id2 = sched.runRepeated(50000, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);

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
	i6e::utils::Clock<TestTimeClock> cl;
	i6e::core::Scheduler<TestTimeClock> sched(cl);

	{
		std::lock_guard<std::mutex> lg(jobLock);
		jobs.clear();
	}

	uint64_t id1 = sched.runRepeated(50001, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(100000, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(150000, boost::bind(func2, 2), i6e::core::JobPriorities::Prio_Medium);
	uint64_t id4 = sched.runRepeated(75001, boost::bind(func2, 3), i6e::core::JobPriorities::Prio_Low);

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
	i6e::utils::Clock<TestTimeClock> cl;
	i6e::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runRepeated(50001, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(100000, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(150000, boost::bind(func2, 2), i6e::core::JobPriorities::Prio_Medium);
	sched.runRepeated(75001, boost::bind(func2, 3), i6e::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 40; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
		if (i == 20) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
			sched.removeTimer(i6e::core::JobPriorities::Prio_Low);
		}
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    std::vector<int> ref({ 0, 3, 1, 2 });
	EXPECT_EQ(ref, jobs);
}

TEST(Scheduler, addTimerLater) {
	i6e::utils::Clock<TestTimeClock> cl;
	i6e::core::Scheduler<TestTimeClock> sched(cl);

	jobs.clear();

	sched.runOnce(50000, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);

	for (int i = 0; i < 6; ++i) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(5));
		cl.passedTime(5000);
		cl.Update();
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	sched.runOnce(15000, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);

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
	i6e::utils::Clock<i6e::utils::RealTimeClock> cl;
	i6e::core::Scheduler<i6e::utils::RealTimeClock> sched(cl);

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));

	sched.runOnce(5000, boost::bind(func2, 0), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runOnce(1500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(3000, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(4500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(2500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);
	sched.runRepeated(3500, boost::bind(func2, 1), i6e::core::JobPriorities::Prio_Low);

	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
}

TEST(Scheduler, startupAndShutdown) {
	for (uint32_t i = 0; i < 100; i++) {
		startClockAndScheduler();
	}
}
