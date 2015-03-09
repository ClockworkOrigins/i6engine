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
	for (size_t i = 0; i < 10000; i++) {
		threads.push_back(new boost::thread(doThreadingStuff));
	}
	for (size_t i = 0; i < 10000; i++) {
		threads[i]->join();
		delete threads[i];
	}
}

TEST(STL, Thread) {
	std::vector<std::thread *> threads;
	for (size_t i = 0; i < 10000; i++) {
		threads.push_back(new std::thread(doThreadingStuff));
	}
	for (size_t i = 0; i < 10000; i++) {
		threads[i]->join();
		delete threads[i];
	}
}
