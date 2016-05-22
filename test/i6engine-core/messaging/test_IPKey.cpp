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

#include "i6engine/core/messaging/IPKey.h"

#include "gtest/gtest.h"

using i6e::core::IPKey;

TEST(IPKey, Equality) {
	IPKey k1("127.0.0.1", 4711);
	IPKey k2("127.0.0.1", 4711);
	IPKey k3("192.168.1.1", 4711);
	IPKey k4("127.0.0.1", 1337);
	IPKey k5, k6;

	EXPECT_EQ(k1, k2);
	EXPECT_EQ(k5, k6);

	EXPECT_NE(k1, k3);
	EXPECT_NE(k1, k4);
	EXPECT_NE(k3, k4);
	EXPECT_NE(k1, k5);
}

TEST(IPKey, isValid) {
	IPKey valid("127.0.0.1", 4711);
	IPKey invalid;

	EXPECT_TRUE(valid.isValid());
	EXPECT_FALSE(invalid.isValid());
}
