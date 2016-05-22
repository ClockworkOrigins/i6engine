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
