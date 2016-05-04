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

#include "i6engine/i6engineBuildSettings.h"
#include "i6engine/utils/Future.h"

#include <thread>

#include "gtest/gtest.h"

std::shared_ptr<i6e::utils::Future<int>> waitAndReturnValue(int value) {
	std::shared_ptr<i6e::utils::Future<int>> ret = std::make_shared<i6e::utils::Future<int>>();
	std::thread([ret, value]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		ret->push(value);
	}).detach();
	return ret;
}

TEST(Future, get) {
	auto future = waitAndReturnValue(42);

	EXPECT_EQ(42, future->get());
}
