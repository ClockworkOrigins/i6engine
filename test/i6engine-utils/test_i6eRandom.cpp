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

#include "i6engine/utils/i6eRandom.h"

#include "gtest/gtest.h"

TEST(i6eRandom, rand) {
	const uint32_t RUNS = 1000000;
	for (uint32_t i = 0; i < RUNS; ++i) {
		uint32_t r = i6e::utils::Random::GetSingletonPtr()->rand(1000);

		EXPECT_LT(r, 1000U);
	}
	for (uint32_t i = 0; i < RUNS; ++i) {
		uint32_t r = i6e::utils::Random::GetSingletonPtr()->rand(111);

		EXPECT_LT(r, 111U);
	}

	EXPECT_EQ(0, i6e::utils::Random::GetSingletonPtr()->rand(1));
	ASSERT_THROW(i6e::utils::Random::GetSingletonPtr()->rand(0), i6e::utils::exceptions::ApiException);

	for (uint32_t i = 0; i < RUNS; ++i) {
		uint32_t r = i6e::utils::Random::GetSingletonPtr()->rand(111, 999);

		EXPECT_TRUE(r >= 111 && r < 999);
	}

	ASSERT_THROW(i6e::utils::Random::GetSingletonPtr()->rand(0, 0), i6e::utils::exceptions::ApiException);
	ASSERT_THROW(i6e::utils::Random::GetSingletonPtr()->rand(5, 2), i6e::utils::exceptions::ApiException);
}

TEST(i6eRandom, setSeed) {
	const uint32_t RUNS = 1000;

	std::vector<uint32_t> noSeedFirst;
	std::vector<uint32_t> noSeedSecond;

	for (uint32_t i = 0; i < RUNS; ++i) {
		noSeedFirst.push_back(i6e::utils::Random::GetSingletonPtr()->rand(1000));
	}
	for (uint32_t i = 0; i < RUNS; ++i) {
		noSeedSecond.push_back(i6e::utils::Random::GetSingletonPtr()->rand(1000));
	}

	EXPECT_FALSE(std::equal(noSeedFirst.begin(), noSeedFirst.end(), noSeedSecond.begin()));

	std::vector<uint32_t> seedFirst;
	std::vector<uint32_t> seedSecond;

	i6e::utils::Random::GetSingleton().setSeed(42);
	for (uint32_t i = 0; i < RUNS; ++i) {
		seedFirst.push_back(i6e::utils::Random::GetSingletonPtr()->rand(1000));
	}
	i6e::utils::Random::GetSingleton().setSeed(42);
	for (uint32_t i = 0; i < RUNS; ++i) {
		seedSecond.push_back(i6e::utils::Random::GetSingletonPtr()->rand(1000));
	}

	EXPECT_TRUE(std::equal(seedFirst.begin(), seedFirst.end(), seedSecond.begin()));
}
