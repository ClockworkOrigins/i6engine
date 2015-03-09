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

#include "i6engine/utils/DoubleBufferQueue.h"

#include "boost/thread.hpp"

#include "gtest/gtest.h"

using i6engine::utils::DoubleBufferQueue;

TEST(DoubleBufferQueue, Simple) {
	DoubleBufferQueue<int, false, false> q;
	EXPECT_TRUE(q.empty());
	EXPECT_EQ(0, q.size());
}

TEST(DoubleBufferQueue, Pushing) {
	DoubleBufferQueue<int, false, false> q;
	for (int i = 0; i < 10; ++i) {
		q.push(i);
		EXPECT_FALSE(q.empty());
		EXPECT_EQ(i + 1, q.size());
	}
}

TEST(DoubleBufferQueue, PushPop) {
	DoubleBufferQueue<int, false, false> q;
	for (int i = 0; i < 10; ++i) {
		q.push(i);
		EXPECT_FALSE(q.empty());
		EXPECT_EQ(i + 1, q.size());
		EXPECT_EQ(0, q.front());
	}
	for (int i = 0; i < 10; ++i) {
		EXPECT_EQ(i, q.front());
		q.pop();
		EXPECT_EQ(9 - i, q.size());
	}
	EXPECT_TRUE(q.empty());
}

TEST(DoubleBufferQueue, Poll) {
	DoubleBufferQueue<int, false, false> q;
	for (int i = 0; i < 10; ++i) {
		q.push(i);
		EXPECT_FALSE(q.empty());
		EXPECT_EQ(i + 1, q.size());
		EXPECT_EQ(0, q.front());
	}
	for (int i = 0; i < 10; ++i) {
		int a = q.poll();
		EXPECT_EQ(i, a);
		EXPECT_EQ(9 - i, q.size());
	}
	EXPECT_TRUE(q.empty());
}

TEST(DoubleBufferQueue, PushPoll2) {
	DoubleBufferQueue<int, false, false> q;
	for (int i = 0; i < 5; ++i) {
		q.push(7);
		EXPECT_EQ(1, q.size());
		q.push(56);
		EXPECT_EQ(2, q.size());
		EXPECT_EQ(7, q.poll());
		EXPECT_EQ(1, q.size());
		q.push(23);
		EXPECT_EQ(2, q.size());
		EXPECT_EQ(56, q.poll());
		EXPECT_EQ(23, q.poll());
		EXPECT_TRUE(q.empty());
	}
}

TEST(DoubleBufferQueue, Clear) {
	DoubleBufferQueue<int, false, false> q;
	q.clear();
	for (int i = 0; i < 5; ++i) {
		q.push(13);
		q.push(37);
		q.push(23);
		q.push(7);
		q.push(42);
		EXPECT_EQ(5, q.size());
		EXPECT_FALSE(q.empty());
		q.clear();
		EXPECT_EQ(0, q.size());
		EXPECT_TRUE(q.empty());
	}
}

TEST(DoubleBufferQueue, Exception) {
	DoubleBufferQueue<int, false, false> q;
	ASSERT_THROW(q.pop(), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(q.front(), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(q.poll(), i6engine::utils::exceptions::ApiException);
	q.push(23);
	q.pop();
	ASSERT_THROW(q.pop(), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(q.front(), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(q.poll(), i6engine::utils::exceptions::ApiException);
}

void pusher(DoubleBufferQueue<int, true, true> * q, int amount, int value) {
	for (int i = 0; i < amount; ++i) {
		q->push(value);
	}
}

void popper(DoubleBufferQueue<int, true, true> * qFrom, DoubleBufferQueue<int, true, false> * qTo, int amount) {
	for (int i = 0; i < amount; ++i) {
		try {
			int a = qFrom->poll();
			qTo->push(a);
		} catch(i6engine::utils::exceptions::ApiException &) {
			i--;
		}
	}
}

TEST(DoubleBufferQueue, StressTest) {
	DoubleBufferQueue<int, true, true> q1;
	DoubleBufferQueue<int, true, false> q2;
	std::vector<boost::thread *> v;
	for (int i = 0; i < 40; ++i) {
		v.push_back(new boost::thread(boost::bind(pusher, &q1, 10000, i)));
	}
	for (int i = 0; i < 80; ++i) {
		v.push_back(new boost::thread(boost::bind(popper, &q1, &q2, 5000)));
	}
	std::vector<int> counts(40);
	for (unsigned int i = 0; i < 120; ++i) {
		v[i]->join();
		delete v[i];
	}
	for (int i = 0; i < 40 * 10000; ++i) {
		counts[size_t(q2.poll())]++;
	}
	for (unsigned int i = 0; i < 40; ++i) {
		EXPECT_EQ(10000, counts[i]);
	}
	EXPECT_TRUE(q1.empty());
	EXPECT_TRUE(q2.empty());
}
