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

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector2.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "gtest/gtest.h"

TEST(i6eVector2, CTor) {
	Vec2 v1;
	Vec2 v2(4, 5);
	Vec2 v3("-0.5 1.0");

	std::map<std::string, std::string> m;
	m["pos"] = "-0.5 1.0";

	Vec2 v4(m, "pos");

	EXPECT_DOUBLE_EQ(0.0, v1.getX());
	EXPECT_DOUBLE_EQ(0.0, v1.getY());
	EXPECT_DOUBLE_EQ(4.0, v2.getX());
	EXPECT_DOUBLE_EQ(5.0, v2.getY());
	EXPECT_DOUBLE_EQ(-0.5, v3.getX());
	EXPECT_DOUBLE_EQ(1.0, v3.getY());
	EXPECT_DOUBLE_EQ(-0.5, v4.getX());
	EXPECT_DOUBLE_EQ(1.0, v4.getY());
}

TEST(i6eVector2, Plus) {
	Vec2 v1(1, 2);
	Vec2 v2(4, 5);
	Vec2 res(5, 7);

	EXPECT_TRUE(v1 + v2 == res);
	EXPECT_FALSE(v1 + v2 != res);
}

TEST(i6eVector2, Minus) {
	Vec2 v1(1, 2);
	Vec2 v2(4, 5);
	Vec2 res(-3, -3);

	EXPECT_TRUE(v1 - v2 == res);
	EXPECT_FALSE(v1 - v2 != res);
}

TEST(i6eVector2, Equal) {
	Vec2 v1(1.0, 2);
	Vec2 v2(1.1, 2);
	Vec2 v3(1.0 + 1e-16, 2);
	Vec2 v4(1.0 + 1e-04, 2);

	EXPECT_TRUE(v1 != v2);
	EXPECT_TRUE(v1 == v3);
	EXPECT_TRUE(v1 != v4);
	EXPECT_TRUE(v3 != v4);

	EXPECT_FALSE(v1 == v2);
	EXPECT_FALSE(v1 != v3);
	EXPECT_FALSE(v1 == v4);
	EXPECT_FALSE(v3 == v4);
}

TEST(i6eVector2, scalMult) {
	Vec2 v1(1, 2);
	Vec2 res(5.0, 10.0);

	EXPECT_TRUE(v1 * 5.0 == res);
	EXPECT_FALSE(v1 * 5.0 != res);
}

TEST(i6eVector2, scalDiv) {
	Vec2 v1(1, 2);
	Vec2 res(0.2, 0.4);

	EXPECT_TRUE(v1 / 5.0 == res);
	EXPECT_FALSE(v1 / 5.0 != res);
}

TEST(i6eVector2, scalProd) {
	Vec2 v1(1, 2);
	Vec2 v2(4, 5);

	EXPECT_DOUBLE_EQ(14.0, Vec2::scalProd(v1, v2));
}

TEST(i6eVector2, length) {
	Vec2 v1(1, 2);
	Vec2 v2(4, 5);

	EXPECT_DOUBLE_EQ(std::sqrt(5.0), v1.length());
	EXPECT_DOUBLE_EQ(std::sqrt(41.0), v2.length());
}

TEST(i6eVector2, normalize) {
	Vec2 v1(1, 2);
	Vec2 v2(4, 5);
	Vec2 v3(0, 0);
	Vec2 res1(1 / std::sqrt(5.0), 2 / std::sqrt(5.0));
	Vec2 res2(4 / std::sqrt(41.0), 5 / std::sqrt(41.0));

	EXPECT_TRUE(res1 == v1.normalize());
	EXPECT_TRUE(res2 == v2.normalize());
	ASSERT_THROW(v3.normalize(), i6engine::utils::exceptions::ApiException);
}

TEST(i6eVector2, serialize) {
	Vec2 v1(1, 2);
	Vec2 v2(4, 5);

	EXPECT_EQ(v1.getX(), 1);
	EXPECT_EQ(v1.getY(), 2);

	EXPECT_EQ(v2.getX(), 4);
	EXPECT_EQ(v2.getY(), 5);

	std::stringstream objStringStream;
	boost::archive::text_oarchive objOArchive(objStringStream);
	objOArchive << v1;

	std::stringstream objStringStream2;
	Vec2 v3;

	objStringStream2 << objStringStream.str();

	boost::archive::text_iarchive objIArchive(objStringStream2);
	objIArchive >> v3;

	EXPECT_EQ(v1.getX(), v3.getX());
	EXPECT_EQ(v1.getY(), v3.getY());

	std::stringstream objStringStream3;
	boost::archive::text_oarchive objOArchive2(objStringStream3);
	objOArchive2 << v2;

	std::stringstream objStringStream4;
	Vec2 v4;

	objStringStream4 << objStringStream3.str();

	boost::archive::text_iarchive objIArchive2(objStringStream4);
	objIArchive2 >> v4;

	EXPECT_EQ(v2.getX(), v4.getX());
	EXPECT_EQ(v2.getY(), v4.getY());
}
