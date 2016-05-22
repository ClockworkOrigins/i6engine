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
