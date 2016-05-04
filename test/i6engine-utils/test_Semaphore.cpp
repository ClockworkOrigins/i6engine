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

#include "i6engine/utils/Semaphore.h"

#include "boost/bind.hpp"
#include "boost/thread.hpp"

#include "gtest/gtest.h"

unsigned int counter = 0;
i6e::utils::Semaphore sem;

void add() {
	sem.wait();
	unsigned int newCounter = counter + 1;
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	counter = newCounter;
	sem.notify();
}

TEST(Semaphore, lock) {
	std::vector<boost::thread *> v;

	EXPECT_EQ(0, counter);
	for(int i = 0; i < 10; i++) {
		v.push_back(new boost::thread(boost::bind(add)));
	}
	for (size_t i = 0; i < 10; i++) {
		v[i]->join();
		delete v[i];
	}
	EXPECT_EQ(10, counter);
}
