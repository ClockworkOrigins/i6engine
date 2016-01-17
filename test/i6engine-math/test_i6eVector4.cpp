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

#include "i6engine/math/i6eVector4.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "LinearMath/btVector3.h"

#include "gtest/gtest.h"

#include "OGRE/OgreVector4.h"

TEST(i6eVector4, CTor) {
	Vec4 v1;
	Vec4 v2(4.0, 5.0, 6.0, 7.0);
	Vec4 v3(Ogre::Vector4(1.0, 2.0, 3.0, 4.0));
	Vec4 v4(btVector4(1.0, 2.0, 3.0, 4.0));

	EXPECT_DOUBLE_EQ(0.0, v1.getX());
	EXPECT_DOUBLE_EQ(0.0, v1.getY());
	EXPECT_DOUBLE_EQ(0.0, v1.getZ());
	EXPECT_DOUBLE_EQ(0.0, v1.getW());
	EXPECT_DOUBLE_EQ(4.0, v2.getW());
	EXPECT_DOUBLE_EQ(5.0, v2.getX());
	EXPECT_DOUBLE_EQ(6.0, v2.getY());
	EXPECT_DOUBLE_EQ(7.0, v2.getZ());
	EXPECT_DOUBLE_EQ(4.0, v3.getW());
	EXPECT_DOUBLE_EQ(1.0, v3.getX());
	EXPECT_DOUBLE_EQ(2.0, v3.getY());
	EXPECT_DOUBLE_EQ(3.0, v3.getZ());
	EXPECT_DOUBLE_EQ(4.0, v4.getW());
	EXPECT_DOUBLE_EQ(1.0, v4.getX());
	EXPECT_DOUBLE_EQ(2.0, v4.getY());
	EXPECT_DOUBLE_EQ(3.0, v4.getZ());
}

TEST(i6eVector4, Addition) {
	Vec4 v1(1.0, 2.0, 3.0, 4.0);
	Vec4 v2(4.0, 3.0, 2.0, 1.0);

	EXPECT_EQ(Vec4(5.0, 5.0, 5.0, 5.0), v1 + v2);
	EXPECT_EQ(Vec4(5.0, 5.0, 5.0, 5.0), v2 + v1);
}

TEST(i6eVector4, Multiplication) {
	Vec4 v1(1.0, 2.0, 3.0, 4.0);

	EXPECT_EQ(Vec4(0.5, 1.0, 1.5, 2.0), v1 * 0.5);
}

TEST(i6eVector4, serialize) {
	Vec4 v1(1.0, 2.0, 3.0, 4.0);
	Vec4 v2(4.0, 5.0, 6.0, 7.0);

	EXPECT_EQ(1.0, v1.getW());
	EXPECT_EQ(2.0, v1.getX());
	EXPECT_EQ(3.0, v1.getY());
	EXPECT_EQ(4.0, v1.getZ());

	EXPECT_EQ(4.0, v2.getW());
	EXPECT_EQ(5.0, v2.getX());
	EXPECT_EQ(6.0, v2.getY());
	EXPECT_EQ(7.0, v2.getZ());

	std::stringstream objStringStream;
	boost::archive::text_oarchive objOArchive(objStringStream);
	objOArchive << v1;

	std::stringstream objStringStream2;
	Vec4 v3;

	objStringStream2 << objStringStream.str();

	boost::archive::text_iarchive objIArchive(objStringStream2);
	objIArchive >> v3;

	EXPECT_EQ(v1.getX(), v3.getX());
	EXPECT_EQ(v1.getY(), v3.getY());
	EXPECT_EQ(v1.getZ(), v3.getZ());
	EXPECT_EQ(v1.getW(), v3.getW());

	std::stringstream objStringStream3;
	boost::archive::text_oarchive objOArchive2(objStringStream3);
	objOArchive2 << v2;

	std::stringstream objStringStream4;
	Vec4 v4;

	objStringStream4 << objStringStream3.str();

	boost::archive::text_iarchive objIArchive2(objStringStream4);
	objIArchive2 >> v4;

	EXPECT_EQ(v2.getX(), v4.getX());
	EXPECT_EQ(v2.getY(), v4.getY());
	EXPECT_EQ(v2.getZ(), v4.getZ());
	EXPECT_EQ(v2.getW(), v4.getW());
}
