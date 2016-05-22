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

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "LinearMath/btVector3.h"

#include "gtest/gtest.h"

#include "OGRE/OgreVector3.h"

TEST(i6eVector, CTor) {
	Vec3 v1;
	Vec3 v2(4, 5, 6);
	Vec3 v3(Ogre::Vector3(1.0, 2.0, 3.0));
	Vec3 v4(btVector3(1.0, 2.0, 3.0));
	Vec3 v5("-0.5 1.0 0.66");

	std::map<std::string, std::string> m;
	m["pos"] = "-0.5 1.0 0.66";

	Vec3 v6(m, "pos");

	EXPECT_DOUBLE_EQ(0.0, v1.getX());
	EXPECT_DOUBLE_EQ(0.0, v1.getY());
	EXPECT_DOUBLE_EQ(0.0, v1.getZ());
	EXPECT_DOUBLE_EQ(4.0, v2.getX());
	EXPECT_DOUBLE_EQ(5.0, v2.getY());
	EXPECT_DOUBLE_EQ(6.0, v2.getZ());
	EXPECT_DOUBLE_EQ(1.0, v3.getX());
	EXPECT_DOUBLE_EQ(2.0, v3.getY());
	EXPECT_DOUBLE_EQ(3.0, v3.getZ());
	EXPECT_DOUBLE_EQ(1.0, v4.getX());
	EXPECT_DOUBLE_EQ(2.0, v4.getY());
	EXPECT_DOUBLE_EQ(3.0, v4.getZ());
	EXPECT_DOUBLE_EQ(-0.5, v5.getX());
	EXPECT_DOUBLE_EQ(1.0, v5.getY());
	EXPECT_DOUBLE_EQ(0.66, v5.getZ());
	EXPECT_DOUBLE_EQ(-0.5, v6.getX());
	EXPECT_DOUBLE_EQ(1.0, v6.getY());
	EXPECT_DOUBLE_EQ(0.66, v6.getZ());
}

TEST(i6eVector, Plus) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);
	Vec3 res(5, 7, 9);

	EXPECT_TRUE(v1 + v2 == res);
	EXPECT_FALSE(v1 + v2 != res);
}

TEST(i6eVector, Minus) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);
	Vec3 res(-3, -3, -3);

	EXPECT_TRUE(v1 - v2 == res);
	EXPECT_FALSE(v1 - v2 != res);
}

TEST(i6eVector, Mult) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);
	Vec3 res(-3.0, 6.0, -3.0);

	EXPECT_TRUE(v1 * v2 == res);
	EXPECT_FALSE(v1 * v2 != res);
}

TEST(i6eVector, Equal) {
	Vec3 v1(1.0, 2, 3);
	Vec3 v2(1.1, 2, 3);
	Vec3 v3(1.0 + 1e-16, 2, 3);
	Vec3 v4(1.0 + 1e-14, 2, 3);

	EXPECT_TRUE(v1 != v2);
	EXPECT_TRUE(v1 == v3);
	EXPECT_TRUE(v1 != v4);
	EXPECT_TRUE(v3 != v4);

	EXPECT_FALSE(v1 == v2);
	EXPECT_FALSE(v1 != v3);
	EXPECT_FALSE(v1 == v4);
	EXPECT_FALSE(v3 == v4);
}

TEST(i6eVector, scalMult) {
	Vec3 v1(1, 2, 3);
	Vec3 res(5.0, 10.0, 15.0);

	EXPECT_TRUE(v1 * 5.0 == res);
	EXPECT_FALSE(v1 * 5.0 != res);
}

TEST(i6eVector, scalDiv) {
	Vec3 v1(1, 2, 3);
	Vec3 res(0.2, 0.4, 0.6);

	EXPECT_TRUE(v1 / 5.0 == res);
	EXPECT_FALSE(v1 / 5.0 != res);
}

TEST(i6eVector, scalProd) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);
	Vec3 res(4, 10, 18);

	EXPECT_DOUBLE_EQ(32.0, Vec3::scalProd(v1, v2));
}

TEST(i6eVector, length) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);

	EXPECT_DOUBLE_EQ(std::sqrt(14.0), v1.length());
	EXPECT_DOUBLE_EQ(std::sqrt(77.0), v2.length());
}

TEST(i6eVector, normalize) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);
	Vec3 v3(0, 0, 0);
	Vec3 res1(1 / std::sqrt(14.0), 2 / std::sqrt(14.0), 3 / std::sqrt(14.0));
	Vec3 res2(4 / std::sqrt(77.0), 5 / std::sqrt(77.0), 6 / std::sqrt(77.0));

	EXPECT_TRUE(res1 == v1.normalize());
	EXPECT_TRUE(res2 == v2.normalize());
	ASSERT_THROW(v3.normalize(), i6e::utils::exceptions::ApiException);
}

TEST(i6eVector, crossAngle) {
	Vec3 v1(1, 0, 0);
	Vec3 v2(0, 1, 0);
	Vec3 v3(0, 0, 1);
	Vec3 v4(0, 0, -1);
	Vec3 v5(0, 0, 0);
	Vec3 v6(0, -1, 0);
	Vec3 v7(-0.24431571847994482, 0, 0.96969574027288097);
	Vec3 v8(-244.31423611485789, 0, 969.68985672768906);

	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngle(v1, v2));
	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngle(v2, v1));
	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngleSigned(v1, v2, v3));
	EXPECT_DOUBLE_EQ(PI / -2.0, Vec3::crossAngleSigned(v2, v1, v3));
	EXPECT_DOUBLE_EQ(PI / -2.0, Vec3::crossAngleSigned(v1, v2, v4));
	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngleSigned(v2, v1, v4));

	EXPECT_DOUBLE_EQ(0.0, Vec3::crossAngle(v5, v5));
	EXPECT_DOUBLE_EQ(0.0, Vec3::crossAngle(v1, v5));
	EXPECT_DOUBLE_EQ(0.0, Vec3::crossAngle(v5, v2));
	EXPECT_DOUBLE_EQ(0.0, Vec3::crossAngleSigned(v1, v5, v3));

	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngle(v1, v6));
	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngle(v6, v1));
	EXPECT_DOUBLE_EQ(PI / -2.0, Vec3::crossAngleSigned(v1, v6, v3));
	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngleSigned(v6, v1, v3));
	EXPECT_DOUBLE_EQ(PI / 2.0, Vec3::crossAngleSigned(v1, v6, v4));
	EXPECT_DOUBLE_EQ(PI / -2.0, Vec3::crossAngleSigned(v6, v1, v4));

	EXPECT_DOUBLE_EQ(PI, Vec3::crossAngle(v3, v4));
	EXPECT_DOUBLE_EQ(PI, Vec3::crossAngle(v4, v3));
	EXPECT_DOUBLE_EQ(PI, Vec3::crossAngleSigned(v3, v4, v2));
	EXPECT_DOUBLE_EQ(PI, Vec3::crossAngleSigned(v4, v3, v2));
	EXPECT_DOUBLE_EQ(PI, Vec3::crossAngleSigned(v3, v4, v6));
	EXPECT_DOUBLE_EQ(PI, Vec3::crossAngleSigned(v4, v3, v6));

	EXPECT_DOUBLE_EQ(0.0, Vec3::crossAngleSigned(v7, v8, v2));
}

TEST(i6eVector, serialize) {
	Vec3 v1(1, 2, 3);
	Vec3 v2(4, 5, 6);

	EXPECT_EQ(v1.getX(), 1);
	EXPECT_EQ(v1.getY(), 2);
	EXPECT_EQ(v1.getZ(), 3);

	EXPECT_EQ(v2.getX(), 4);
	EXPECT_EQ(v2.getY(), 5);
	EXPECT_EQ(v2.getZ(), 6);

	std::stringstream objStringStream;
	boost::archive::text_oarchive objOArchive(objStringStream);
	objOArchive << v1;

	std::stringstream objStringStream2;
	Vec3 v3;

	objStringStream2 << objStringStream.str();

	boost::archive::text_iarchive objIArchive(objStringStream2);
	objIArchive >> v3;

	EXPECT_EQ(v1.getX(), v3.getX());
	EXPECT_EQ(v1.getY(), v3.getY());
	EXPECT_EQ(v1.getZ(), v3.getZ());

	std::stringstream objStringStream3;
	boost::archive::text_oarchive objOArchive2(objStringStream3);
	objOArchive2 << v2;

	std::stringstream objStringStream4;
	Vec3 v4;

	objStringStream4 << objStringStream3.str();

	boost::archive::text_iarchive objIArchive2(objStringStream4);
	objIArchive2 >> v4;

	EXPECT_EQ(v2.getX(), v4.getX());
	EXPECT_EQ(v2.getY(), v4.getY());
	EXPECT_EQ(v2.getZ(), v4.getZ());
}
