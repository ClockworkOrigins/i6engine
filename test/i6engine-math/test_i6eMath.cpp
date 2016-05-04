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
