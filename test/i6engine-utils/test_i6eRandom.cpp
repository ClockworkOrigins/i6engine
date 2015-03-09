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

#include "i6engine/utils/i6eRandom.h"

#include "gtest/gtest.h"

TEST(i6eRandom, rand) {
	const uint32_t RUNS = 1000000;
	for (uint32_t i = 0; i < RUNS; ++i) {
		uint32_t r = i6engine::utils::Random::GetSingletonPtr()->rand(1000);

		EXPECT_LT(r, 1000U);
	}
	for (uint32_t i = 0; i < RUNS; ++i) {
		uint32_t r = i6engine::utils::Random::GetSingletonPtr()->rand(111);

		EXPECT_LT(r, 111U);
	}

	EXPECT_EQ(0, i6engine::utils::Random::GetSingletonPtr()->rand(1));
	ASSERT_THROW(i6engine::utils::Random::GetSingletonPtr()->rand(0), i6engine::utils::exceptions::ApiException);

	for (uint32_t i = 0; i < RUNS; ++i) {
		uint32_t r = i6engine::utils::Random::GetSingletonPtr()->rand(111, 999);

		EXPECT_TRUE(r >= 111 && r < 999);
	}

	ASSERT_THROW(i6engine::utils::Random::GetSingletonPtr()->rand(0, 0), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(i6engine::utils::Random::GetSingletonPtr()->rand(5, 2), i6engine::utils::exceptions::ApiException);
}
