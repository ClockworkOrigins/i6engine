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

using i6e::utils::RangedMap;

TEST(RangedMap, comparisonMapToRangedMap) {
	const int ENTRIES = 3000;
	const int RUNS = 250;

	std::vector<int> vec(ENTRIES);
	std::map<int, int> nm;

	uint64_t endTime;
	uint64_t startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (int i = 0; i < RUNS; i++) {
		for (int j = 0; j < ENTRIES; j++) {
			vec[size_t(j)] = int(i6e::utils::Random::GetSingleton().rand(20));
		}

		for (int j = 0; j < ENTRIES; j++) {
			nm[j] = vec[size_t(j)];
		}

		for (int j = 0; j < ENTRIES; j++) {
			EXPECT_EQ(vec[size_t(j)], nm[j]);
		}
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationNM = endTime - startTime;
	RangedMap<int, int> rm(0, ENTRIES, -1);

	startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (int i = 0; i < RUNS; i++) {
		for (int j = 0; j < ENTRIES; j++) {
			vec[size_t(j)] = int(i6e::utils::Random::GetSingleton().rand(20));
		}

		for (int j = 0; j < ENTRIES; j++) {
			rm.set(j, vec[size_t(j)]);
		}

		for (int j = 0; j < ENTRIES; j++) {
			EXPECT_EQ(vec[size_t(j)], rm.get(j));
		}
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationRM = endTime - startTime;

	EXPECT_LE(durationRM, durationNM);
	RecordProperty("Duration Ranged Map", int(durationRM));
	RecordProperty("Duration std Map", int(durationNM));

	std::cout << "RangedMap: " << durationRM << std::endl;
	std::cout << "std::map: " << durationNM << std::endl;
}

TEST(RangedMap, comparisonMapToRangedMap2) {
	const int ENTRIES = 1000000;
	const int RUNS = 250000;

	std::map<int, int> nm;

	uint64_t endTime;
	uint64_t startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (int i = 0; i < RUNS; i++) {
		int rnd = int(i6e::utils::Random::GetSingleton().rand(ENTRIES));

		if (nm[rnd] < 5) {
			nm[rnd]++;
		}
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationNM = endTime - startTime;

	RangedMap<int, int> rm(0, ENTRIES, 0);

	startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (int i = 0; i < RUNS; i++) {
		int rnd = int(i6e::utils::Random::GetSingleton().rand(ENTRIES));

		if (rm.get(rnd) < 5) {
			rm.set(rnd, rm.get(rnd) + 1);
		}
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationRM = endTime - startTime;

	EXPECT_LE(durationRM, durationNM);
	RecordProperty("Duration Ranged Map", int(durationRM));
	RecordProperty("Duration std Map", int(durationNM));

	std::cout << "RangedMap: " << durationRM << std::endl;
	std::cout << "std::map: " << durationNM << std::endl;
}

TEST(RangedMap, comparisonMapToRangedMapSet) {
	const size_t ENTRIES = 100000;

	std::set<int> orderSet;
	std::vector<int> orderList;

	while (orderSet.size() < ENTRIES) {
		int rnd = int(i6e::utils::Random::GetSingleton().rand(ENTRIES));
		size_t before = orderSet.size();
		orderSet.insert(rnd);
		if (before < orderSet.size()) {
			orderList.push_back(rnd);
		}
	}

	std::map<int, int> nm;

	uint64_t endTime;
	uint64_t startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (size_t i = 0; i < orderList.size(); i++) {
		nm[orderList[i]] = 1;
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationNM = endTime - startTime;

	RangedMap<int, int> rm(0, ENTRIES, 0);

	startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (size_t i = 0; i < orderList.size(); i++) {
		rm.set(orderList[i], 1);
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationRM = endTime - startTime;

	EXPECT_LE(durationRM, durationNM);
	RecordProperty("Duration Ranged Map", int(durationRM));
	RecordProperty("Duration std Map", int(durationNM));

	std::cout << "RangedMap: " << durationRM << std::endl;
	std::cout << "std::map: " << durationNM << std::endl;
}

TEST(RangedMap, comparisonMapToRangedMapGet) {
	const size_t ENTRIES = 100000;

	std::set<int> orderSet;
	std::vector<std::pair<int, int>> orderList;

	while (orderSet.size() < ENTRIES) {
		int rnd = int(i6e::utils::Random::GetSingleton().rand(ENTRIES));
		size_t before = orderSet.size();
		orderSet.insert(rnd);
		if (before < orderSet.size()) {
			orderList.push_back(std::make_pair(rnd, rnd % 6));
		}
	}

	std::map<int, int> nm;
	RangedMap<int, int> rm(0, ENTRIES, 0);

	for (size_t i = 0; i < orderList.size(); i++) {
		auto p = orderList[i];
		nm[p.first] = p.second;
		rm.set(p.first, p.second);
	}

	uint64_t endTime;
	uint64_t startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (size_t i = 0; i < orderList.size(); i++) {
		EXPECT_EQ(orderList[i].second, nm[orderList[i].first]);
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationNM = endTime - startTime;

	startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	for (size_t i = 0; i < orderList.size(); i++) {
		EXPECT_EQ(orderList[i].second, rm.get(orderList[i].first));
	}
	endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	uint64_t durationRM = endTime - startTime;

	EXPECT_LE(durationRM, durationNM);
	RecordProperty("Duration Ranged Map", int(durationRM));
	RecordProperty("Duration std Map", int(durationNM));

	std::cout << "RangedMap: " << durationRM << std::endl;
	std::cout << "std::map: " << durationNM << std::endl;
}
