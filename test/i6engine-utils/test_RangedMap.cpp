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

#include "i6engine/utils/RangedMap.h"

#include <chrono>

#include "i6engine/utils/i6eRandom.h"

#include "gtest/gtest.h"

using i6engine::utils::RangedMap;

TEST(RangedMap, getAndSet) {
	RangedMap<int, bool> rm(0, 1000, false);

	for (int i = 0; i <= 1000; i++) {
		EXPECT_FALSE(rm.get(i));
	}

	rm.set(0, true);

	EXPECT_TRUE(rm.get(0));

	for (int i = 1; i <= 1000; i++) {
		EXPECT_FALSE(rm.get(i));
	}

	rm.set(599, true);
	EXPECT_TRUE(rm.get(0));
	EXPECT_TRUE(rm.get(599));

	for (int i = 1; i <= 1000; i++) {
		if (i == 599) {
			continue;
		}
		EXPECT_FALSE(rm.get(i));
	}

	rm.set(599, false);
	rm.set(0, false);
	EXPECT_FALSE(rm.get(0));
	EXPECT_FALSE(rm.get(599));

	for (int i = 0; i <= 1000; i++) {
		EXPECT_FALSE(rm.get(i));
	}

	for (int i = 0; i <= 1000; i++) {
		rm.set(i, true);
		EXPECT_TRUE(rm.get(i));
	}

	for (int i = 0; i <= 1000; i++) {
		rm.set(i, false);
		EXPECT_FALSE(rm.get(i));
	}
}

TEST(RangedMap, randomSet) {
	RangedMap<int, bool> rm(INT_MIN, INT_MAX, false);

	std::map<int, bool> m;

	for (int i = 0; i < 10000; i++) {
		int rnd = int(i6engine::utils::Random::GetSingleton().rand(0u, UINT_MAX));
		m[rnd] = true;
		rm.set(rnd, true);
	}

	for (int i = INT_MIN / 1000; i < INT_MAX / 1000; i++) {
		if (m.find(i) == m.end()) {
			EXPECT_FALSE(rm.get(i));
		} else {
			EXPECT_TRUE(rm.get(i));
		}
	}
}

TEST(RangedMap, multipleMerges) {
	const size_t ENTRIES = 10000;
	RangedMap<int, bool> rm(0, ENTRIES, false);

	std::vector<int> vec(ENTRIES);

	for (size_t i = 0; i < ENTRIES; i++) {
		vec[size_t(i)] = int(i);
	}

	std::random_shuffle(vec.begin(), vec.end());

	for (size_t i = 0; i < ENTRIES; i++) {
		EXPECT_FALSE(rm.get(vec[i]));
		rm.set(vec[i], true);
	}

	for (size_t i = 0; i < ENTRIES; i++) {
		EXPECT_TRUE(rm.get(int(i)));
	}

	std::random_shuffle(vec.begin(), vec.end());

	for (size_t i = 0; i < ENTRIES; i++) {
		rm.set(vec[i], false);
		EXPECT_FALSE(rm.get(vec[i]));
	}

	for (size_t i = 0; i < ENTRIES; i++) {
		EXPECT_FALSE(rm.get(int(i)));
	}
}

TEST(RangedMap, specialMultipleMerges) {
	const size_t ENTRIES = 20;
	RangedMap<int, bool> rm(0, ENTRIES, false);

	std::vector<int> vec = {19, 14, 12, 18, 9, 2, 6, 5, 16, 8, 1, 7, 15, 3, 13, 0, 4, 10, 11, 17};

	for (size_t i = 0; i < ENTRIES; i++) {
		EXPECT_FALSE(rm.get(vec[i]));
		rm.set(vec[i], true);
	}

	for (size_t i = 0; i < ENTRIES; i++) {
		EXPECT_TRUE(rm.get(int(i)));
	}
}

TEST(RangedMap, randomValues) {
	const int ENTRIES = 10000;
	RangedMap<int, int> rm(0, ENTRIES, -1);

	std::vector<int> vec(ENTRIES);

	for (int i = 0; i < ENTRIES; i++) {
		vec[size_t(i)] = int(i6engine::utils::Random::GetSingleton().rand(20));
	}

	for (int i = 0; i < ENTRIES; i++) {
		EXPECT_EQ(-1, rm.get(i));
		rm.set(i, vec[size_t(i)]);
	}

	for (int i = 0; i < ENTRIES; i++) {
		EXPECT_EQ(vec[size_t(i)], rm.get(i));
	}

	for (int i = 0; i < ENTRIES; i++) {
		rm.set(i, -1);
		EXPECT_EQ(-1, rm.get(i));
	}

	for (int i = 0; i < ENTRIES; i++) {
		EXPECT_EQ(-1, rm.get(i));
	}
}

TEST(RangedMap, stressTest) {
	const int ENTRIES = 3000;
	RangedMap<int, int> rm(0, ENTRIES, -1);

	std::vector<int> vec(ENTRIES);

	for (int j = 0; j < 500; j++) {
		for (int i = 0; i < ENTRIES; i++) {
			vec[size_t(i)] = int(i6engine::utils::Random::GetSingleton().rand(20));
		}

		for (int i = 0; i < ENTRIES; i++) {
			rm.set(i, vec[size_t(i)]);
			EXPECT_EQ(vec[size_t(i)], rm.get(i));
		}

		for (int i = 0; i < ENTRIES; i++) {
		}
	}
}

TEST(RangedMap, sequenceTest) {
	const int ENTRIES = 10;

	std::vector<std::vector<int>> vec {
		{1, 2, 4, 0, 4, 3, 0, 3, 3, 0,
		2, 1, 3, 0, 3, 3, 3, 4, 0, 3,
		1, 2, 1, 0, 1, 3, 4, 4, 3, 1,
		3, 0, 0, 4, 4, 4, 4, 0, 0, 3,
		2, 1, 1, 2, 2, 1, 1, 0, 3, 2},

		{17, 6, 9, 6, 8, 7, 13, 17, 17, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

		{6, 19, 4, 4, 13, 11, 19, 1, 7, 18,
		3, 10, 10, 18, 8, 1, 14, 1, 8, 8,
		19, 14, 12, 13, 7, 10, 8, 2, 8, 7,
		6, 0, 14, 12, 5, 4, 4, 15, 16, 0,
		9, 1, 9, 3, 2, 14, 3, 19, 12, 8,
		1, 15, 13, 9, 9, 15, 10, 7, 14, 6},

		{1, 2, 4, 0, 4, 3, 0, 3, 3, 0,
		2, 1, 3, 0, 3, 3, 3, 4, 0, 3,
		1, 2, 1, 0, 1, 3, 4, 4, 3, 1,
		3, 0, 0, 4, 4, 4, 4, 0, 0, 3,
		2, 1, 1, 2, 2, 1, 1, 0, 3, 2,
		17, 6, 9, 6, 8, 7, 13, 17, 17, 11,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		6, 19, 4, 4, 13, 11, 19, 1, 7, 18,
		3, 10, 10, 18, 8, 1, 14, 1, 8, 8,
		19, 14, 12, 13, 7, 10, 8, 2, 8, 7,
		6, 0, 14, 12, 5, 4, 4, 15, 16, 0,
		9, 1, 9, 3, 2, 14, 3, 19, 12, 8,
		1, 15, 13, 9, 9, 15, 10, 7, 14, 6}
	};

	for (size_t i = 0; i < vec.size(); i++) {
		RangedMap<int, int> rm(0, ENTRIES, -1);
		for (size_t j = 0; j < vec[i].size(); j++) {
			rm.set(j % ENTRIES, vec[i][j]);
			EXPECT_EQ(vec[i][j], rm.get(int(j % ENTRIES)));
		}
	}
}

TEST(RangedMap, specialTrees) {
	RangedMap<int, int> rm(0, 10, -1);

	rm.set(0, 0);

//	rm.dump("t1.dot");

	rm.set(2, 1);

//	rm.dump("t2.dot");

	rm.set(1, 0);

//	rm.dump("t3.dot");
}

TEST(RangedMap, specialTrees2) {
	RangedMap<int, int> rm(0, 10, -1);

	rm.set(0, 0);

//	std::cout << "digraph {" << std::endl;
//	rm._head->dump();
//	std::cout << "}" << std::endl;

	rm.set(2, 1);

//	std::cout << "digraph {" << std::endl;
//	rm._head->dump();
//	std::cout << "}" << std::endl;

	rm.set(3, 2);

//	std::cout << "digraph {" << std::endl;
//	rm._head->dump();
//	std::cout << "}" << std::endl;

	rm.set(1, 0);

//	std::cout << "digraph {" << std::endl;
//	rm._head->dump();
//	std::cout << "}" << std::endl;
}
