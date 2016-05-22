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

#include <chrono>
#include <functional>
#include <thread>

#include "i6engine/i6engineBuildSettings.h"

#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/function.hpp"
#include "boost/thread.hpp"

#include "gtest/gtest.h"

int func(int i) {
	return ++i;
}

TEST(Boost, FunctionCall) {
	boost::function<int(int)> f = boost::bind(func, _1);

	int counter = 0;

	for (int i = 0; i < 100000000; i++) {
		counter = f(counter);
	}

	EXPECT_EQ(100000000, counter);
}

TEST(STL, FunctionCall) {
	std::function<int(int)> f = std::bind(func, std::placeholders::_1);

	int counter = 0;

	for (int i = 0; i < 100000000; i++) {
		counter = f(counter);
	}

	EXPECT_EQ(100000000, counter);
}

TEST(Boost, FunctionCopy) {
	boost::function<int(int)> f = boost::bind(func, _1);
	boost::function<int(int)> g;

	for (int i = 0; i < 100000000; i++) {
		g = f;
	}
}

TEST(STL, FunctionCopy) {
	std::function<int(int)> f = std::bind(func, std::placeholders::_1);
	std::function<int(int)> g;

	for (int i = 0; i < 100000000; i++) {
		g = f;
	}
}

TEST(Boost, getTime) {
	boost::posix_time::ptime time;
	for (int i = 0; i < 1000000; i++) {
		time = boost::posix_time::microsec_clock::universal_time();
	}
}

TEST(STL, getTime) {
	std::chrono::time_point<std::chrono::high_resolution_clock> time;
	for (int i = 0; i < 1000000; i++) {
		time = std::chrono::high_resolution_clock::now();
	}
}

void doThreadingStuff() {
	int i = 0;
	i++;
	i = 100;
}

TEST(Boost, Thread) {
	std::vector<boost::thread *> threads;
	for (size_t i = 0; i < 1000; i++) {
		threads.push_back(new boost::thread(doThreadingStuff));
	}
	for (size_t i = 0; i < 1000; i++) {
		threads[i]->join();
		delete threads[i];
	}
}

TEST(STL, Thread) {
	std::vector<std::thread *> threads;
	for (size_t i = 0; i < 1000; i++) {
		threads.push_back(new std::thread(doThreadingStuff));
	}
	for (size_t i = 0; i < 1000; i++) {
		threads[i]->join();
		delete threads[i];
	}
}
