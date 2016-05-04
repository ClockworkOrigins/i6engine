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
#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "LinearMath/btQuaternion.h"

#include "gtest/gtest.h"

#include "OGRE/OgreQuaternion.h"

// Caution: Bullet Quaternions expect W as last parameter, Quaternion as first parameter

TEST(i6eQuaternion, Construction) {
	Quaternion q(4, 5, 6, 7);
	Quaternion q2(q);
	Quaternion q3 = q;

	Vec3 v(1.0, 0.0, 0.0);

	Quaternion q4(v, 90.0);

	ASSERT_THROW(Quaternion(Vec3(1.0, 2.0, 3.0), 90.0), i6e::utils::exceptions::ApiException);

	btQuaternion bq(5, 6, 7, 4);

	EXPECT_TRUE(q.toBullet() == bq);
	EXPECT_TRUE(q == Quaternion(bq));
	EXPECT_TRUE(q2 == q);
	EXPECT_TRUE(q2.toBullet() == bq);
	EXPECT_TRUE(q3.toBullet() == bq);
	EXPECT_TRUE(q3 == q);

	EXPECT_DOUBLE_EQ(std::cos(90.0 / 2), q4.getW());
	EXPECT_DOUBLE_EQ(std::sin(90.0 / 2), q4.getX());
	EXPECT_DOUBLE_EQ(0.0, q4.getY());
	EXPECT_DOUBLE_EQ(0.0, q4.getZ());
}

TEST(i6eQuaternion, AddSub) {
	Quaternion v1(1, 2, 3, 4);
	Quaternion v2(4, 5, 6, 7);
	Quaternion v3 = v1;
	v3 += v2;

	btQuaternion b1(2, 3, 4, 1);
	btQuaternion b2(5, 6, 7, 4);
	btQuaternion b3 = b1;
	b3 += b2;

	EXPECT_TRUE((v1 + v2).toBullet() == (b1 + b2));
	EXPECT_TRUE((v1 - v2).toBullet() == (b1 - b2));
	EXPECT_DOUBLE_EQ(5, v3.getW());
	EXPECT_DOUBLE_EQ(7, v3.getX());
	EXPECT_DOUBLE_EQ(9, v3.getY());
	EXPECT_DOUBLE_EQ(11, v3.getZ());
}

TEST(i6eQuaternion, Multiplication) {
	Quaternion v1(1, 2, 3, 4);
	Quaternion v2(4, 5, 6, 7);

	btQuaternion b1(2, 3, 4, 1);
	btQuaternion b2(5, 6, 7, 4);

	EXPECT_TRUE((v1 * v2).toBullet() == (b1 * b2));
	EXPECT_TRUE((v1 * v2).toBullet() == (b1 * b2));
}

TEST(i6eQuaternion, MultVector) {
	Vec3 v1(1, 2, 3);
	Quaternion v2(4, 5, 6, 7);

	btVector3 b1(1, 2, 3);
	btQuaternion b2(5, 6, 7, 4);

	EXPECT_TRUE((v1 * v2).toBullet() == (b1 * b2));
	EXPECT_TRUE((v2 * v1).toBullet() == (b2 * b1));
}

TEST(i6eQuaternion, Inverse) {
	Quaternion q(4, 5, 6, 7);

	btQuaternion bq(5, 6, 7, 4);

	EXPECT_TRUE(q.inverse().toBullet() == bq.inverse());

	Quaternion a(Vec3(0.0, 1.0, 0.0), PI / 2.0);
	Quaternion b(Vec3(0.0, 1.0, 0.0), PI / 4.0);
	EXPECT_EQ(a * a.inverse() * b, b);
}

TEST(i6eQuaternion, length) {
	Quaternion q(4, 5, 6, 7);

	EXPECT_DOUBLE_EQ(11.224972160321824, q.length());
}

TEST(i6eQuaternion, serialize) {
	Quaternion q1(1, 2, 3, 4);
	Quaternion q2(5, 6, 7, 8);

	EXPECT_EQ(q1.getX(), 2);
	EXPECT_EQ(q1.getY(), 3);
	EXPECT_EQ(q1.getZ(), 4);
	EXPECT_EQ(q1.getW(), 1);

	EXPECT_EQ(q2.getX(), 6);
	EXPECT_EQ(q2.getY(), 7);
	EXPECT_EQ(q2.getZ(), 8);
	EXPECT_EQ(q2.getW(), 5);

	std::stringstream objStringStream;
	boost::archive::text_oarchive objOArchive(objStringStream);
	objOArchive << q1;

	std::stringstream objStringStream2;
	Quaternion q3;

	objStringStream2 << objStringStream.str();

	boost::archive::text_iarchive objIArchive(objStringStream2);
	objIArchive >> q3;

	EXPECT_EQ(q1.getX(), q3.getX());
	EXPECT_EQ(q1.getY(), q3.getY());
	EXPECT_EQ(q1.getZ(), q3.getZ());
	EXPECT_EQ(q1.getW(), q3.getW());

	std::stringstream objStringStream3;
	boost::archive::text_oarchive objOArchive2(objStringStream3);
	objOArchive2 << q2;

	std::stringstream objStringStream4;
	Quaternion q4;

	objStringStream4 << objStringStream3.str();

	boost::archive::text_iarchive objIArchive2(objStringStream4);
	objIArchive2 >> q4;

	EXPECT_EQ(q2.getX(), q4.getX());
	EXPECT_EQ(q2.getY(), q4.getY());
	EXPECT_EQ(q2.getZ(), q4.getZ());
	EXPECT_EQ(q2.getW(), q4.getW());
}

TEST(i6eQuaternion, EqualsOperator) {
	Quaternion q1(1.0, 2.0, 3.0, 4.0);
	Quaternion q2(1.1, 2.0, 3.0, 4.0);
	Quaternion q3(1.0, 2.0, 3.0, 4.0);
	Quaternion q4(2.0, 7.3, 3.0, 4.0);

	EXPECT_FALSE(q1 == q2);
	EXPECT_TRUE(q1 == q3);
	EXPECT_FALSE(q1 == q4);
	EXPECT_FALSE(q2 == q3);
	EXPECT_FALSE(q2 == q4);
	EXPECT_FALSE(q3 == q4);

	EXPECT_TRUE(q1 != q2);
	EXPECT_FALSE(q1 != q3);
	EXPECT_TRUE(q1 != q4);
	EXPECT_TRUE(q2 != q3);
	EXPECT_TRUE(q2 != q4);
	EXPECT_TRUE(q3 != q4);
}

TEST(i6eQuaternion, scalProd) {
	Quaternion q1(1.0, 2.0, 3.0, 4.0);
	Quaternion q2(0.5, -1.0, 0.25, 9.0);

	double d = scalProd(q1, q2);

	EXPECT_DOUBLE_EQ(35.25, d);
}

TEST(i6eQuaternion, EqualRotation) {
	Quaternion q1(1.0, 2.0, 3.0, 4.0);
	Quaternion q2(1.1, 2.0, 3.0, 4.0);
	Quaternion q3(1.0, 2.0, 3.0, 4.0);
	Quaternion q4(2.0, 7.3, 3.0, 4.0);

	EXPECT_FALSE(q1.equals(q2, 1e-15));
	EXPECT_TRUE(q1.equals(q3, 1e-15));
}

TEST(i6eQuaternion, normalize) {
	Quaternion q1(1.0, 2.0, 3.0, 4.0);
	Quaternion q2(1.1, 2.0, 3.0, 4.0);
	Quaternion q3(1.0, 2.0, 3.0, 4.0);
	Quaternion q4(2.0, 7.3, 3.0, 4.0);
	Quaternion q5 = q1.normalize();
	Quaternion q6 = q2.normalize();
	Quaternion q7 = q3.normalize();
	Quaternion q8 = q4.normalize();

	EXPECT_DOUBLE_EQ(1.0, q5.length());
	EXPECT_DOUBLE_EQ(1.0, q6.length());
	EXPECT_DOUBLE_EQ(1.0, q7.length());
	EXPECT_DOUBLE_EQ(1.0, q8.length());
}

TEST(i6eQuaternion, toAxisAngle) {
	Quaternion q1(1.0, 0.0, 0.0, 0.0);
	Quaternion q2(Vec3(1.0, 0.0, 0.0), 180.0 / 90.0 * PI);

	Vec3 v1, v2;
	double a1, a2;

	q1.toAxisAngle(v1, a1);
	q2.toAxisAngle(v2, a2);

	EXPECT_DOUBLE_EQ(0.0, a1);
	EXPECT_DOUBLE_EQ(180.0 / 90.0 * PI, a2);
	EXPECT_EQ(Vec3(0.0, 1.0, 0.0), v1);
	EXPECT_EQ(Vec3(1.0, 0.0, 0.0), v2);
}
