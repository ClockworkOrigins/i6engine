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

#include "i6engine/math/i6eMath.h"

#include "gtest/gtest.h"

TEST(i6eMath, binom) {
	EXPECT_EQ(10, i6e::math::binom(5, 3));
	EXPECT_EQ(10, i6e::math::binom(5, 2));
	EXPECT_EQ(13983816, i6e::math::binom(49, 6));
	EXPECT_EQ(1, i6e::math::binom(20, 0));
	EXPECT_EQ(0, i6e::math::binom(5, 7));
	EXPECT_EQ(33, i6e::math::binom(33, 1));
}

TEST(i6eMath, disPointLine) {
	EXPECT_FLOAT_EQ(2.5123074f, float(i6e::math::disPointLine(Vec3(1.0, 2.0, 3.0), Vec3(4.0, 5.0, 6.0), Vec3(6.0, 6.0, 6.0))));
	EXPECT_FLOAT_EQ(5.0241838f, float(i6e::math::disPointLine(Vec3(1.0, 0.0, 1.0), Vec3(2.0, 5.0, 2.0), Vec3(5.0, 3.0, -2.0))));
	ASSERT_THROW(i6e::math::disPointLine(Vec3(1.0, 0.0, 1.0), Vec3(0.0, 0.0, 0.0), Vec3(5.0, 3.0, -2.0)), i6e::utils::exceptions::ApiException);
}

TEST(i6eMath, rotateVector) {
	Vec3 v(1.0, 2.0, 3.0);
	Quaternion q(1.0, 0.0, 1.0, 0.0);

	EXPECT_EQ(Vec3(6.0, 4.0, -2.0), i6e::math::rotateVector(v, q));
}
