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

#include "i6engine/utils/sequence_map.h"

#include "gtest/gtest.h"

TEST(sequence_map, insert) {
	i6e::utils::sequence_map<std::string, int> testMap1;

	testMap1["FirstEntry"] = 0;
	testMap1["SecondEntry"] = 1;
	testMap1["ThirdEntry"] = 3;
	testMap1["FourthEntry"] = 5;
	testMap1["FifthEntry"] = 4;

	ASSERT_EQ(5, testMap1.size());

	EXPECT_EQ(0, testMap1["FirstEntry"]);
	EXPECT_EQ(1, testMap1["SecondEntry"]);
	EXPECT_EQ(3, testMap1["ThirdEntry"]);
	EXPECT_EQ(5, testMap1["FourthEntry"]);
	EXPECT_EQ(4, testMap1["FifthEntry"]);

	testMap1["SecondEntry"] = 6;

	ASSERT_EQ(5, testMap1.size());

	EXPECT_EQ(6, testMap1["SecondEntry"]);
}

TEST(sequence_map, iterator) {
	i6e::utils::sequence_map<std::string, int> testMap1;

	testMap1["FirstEntry"] = 0;
	testMap1["SecondEntry"] = 1;
	testMap1["ThirdEntry"] = 3;
	testMap1["FourthEntry"] = 5;
	testMap1["FifthEntry"] = 4;

	std::vector<std::string> v = { "FirstEntry", "SecondEntry", "ThirdEntry", "FourthEntry", "FifthEntry" };

	ASSERT_EQ(v.size(), testMap1.size());

	unsigned int i = 0;

	for (std::pair<std::string, int> p : testMap1) {
		EXPECT_EQ(v[i], p.first);
		i++;
	}
}

TEST(sequence_map, erase) {
	i6e::utils::sequence_map<std::string, int> testMap1;

	testMap1["FirstEntry"] = 0;
	testMap1["SecondEntry"] = 1;
	testMap1["ThirdEntry"] = 3;
	testMap1["FourthEntry"] = 5;
	testMap1["FifthEntry"] = 4;

	std::vector<std::string> v = { "FirstEntry", "SecondEntry", "ThirdEntry", "FourthEntry", "FifthEntry" };

	ASSERT_EQ(v.size(), testMap1.size());

	unsigned int i = 0;

	for (std::pair<std::string, int> p : testMap1) {
		ASSERT_EQ(v[i], p.first);
		i++;
	}

	testMap1.erase("ThirdEntry");

	std::vector<std::string> v2 = { "FirstEntry", "SecondEntry", "FourthEntry", "FifthEntry" };

	ASSERT_EQ(v2.size(), testMap1.size());

	i = 0;

	for (std::pair<std::string, int> p : testMap1) {
		ASSERT_EQ(v2[i], p.first);
		i++;
	}

	testMap1.erase("ThirdEntry");

	ASSERT_EQ(v2.size(), testMap1.size());

	i = 0;

	for (std::pair<std::string, int> p : testMap1) {
		ASSERT_EQ(v2[i], p.first);
		i++;
	}

	testMap1.erase("FirstEntry");

	std::vector<std::string> v3 = { "SecondEntry", "FourthEntry", "FifthEntry" };

	ASSERT_EQ(v3.size(), testMap1.size());

	i = 0;

	for (std::pair<std::string, int> p : testMap1) {
		ASSERT_EQ(v3[i], p.first);
		i++;
	}

	testMap1.erase("FifthEntry");

	std::vector<std::string> v4 = { "SecondEntry", "FourthEntry" };

	ASSERT_EQ(v4.size(), testMap1.size());

	i = 0;

	for (std::pair<std::string, int> p : testMap1) {
		ASSERT_EQ(v4[i], p.first);
		i++;
	}
}

TEST(sequence_map, clear) {
	i6e::utils::sequence_map<std::string, int> testMap1;

	testMap1["FirstEntry"] = 0;
	testMap1["SecondEntry"] = 1;
	testMap1["ThirdEntry"] = 3;
	testMap1["FourthEntry"] = 5;
	testMap1["FifthEntry"] = 4;

	ASSERT_EQ(5, testMap1.size());

	EXPECT_EQ(0, testMap1["FirstEntry"]);
	EXPECT_EQ(1, testMap1["SecondEntry"]);
	EXPECT_EQ(3, testMap1["ThirdEntry"]);
	EXPECT_EQ(5, testMap1["FourthEntry"]);
	EXPECT_EQ(4, testMap1["FifthEntry"]);

	testMap1.clear();

	ASSERT_EQ(0, testMap1.size());

	EXPECT_TRUE(testMap1.find("FirstEntry") == testMap1.end());
	EXPECT_TRUE(testMap1.find("SecondEntry") == testMap1.end());
	EXPECT_TRUE(testMap1.find("ThirdEntry") == testMap1.end());
	EXPECT_TRUE(testMap1.find("FourthEntry") == testMap1.end());
	EXPECT_TRUE(testMap1.find("FifthEntry") == testMap1.end());
	EXPECT_TRUE(testMap1.find("SixthEntry") == testMap1.end());
}

TEST(sequence_map, find) {
	i6e::utils::sequence_map<std::string, int> testMap1;

	testMap1["FirstEntry"] = 0;
	testMap1["SecondEntry"] = 1;
	testMap1["ThirdEntry"] = 3;
	testMap1["FourthEntry"] = 5;
	testMap1["FifthEntry"] = 4;

	ASSERT_EQ(5, testMap1.size());

	EXPECT_EQ(0, testMap1["FirstEntry"]);
	EXPECT_EQ(1, testMap1["SecondEntry"]);
	EXPECT_EQ(3, testMap1["ThirdEntry"]);
	EXPECT_EQ(5, testMap1["FourthEntry"]);
	EXPECT_EQ(4, testMap1["FifthEntry"]);

	EXPECT_TRUE(testMap1.find("FirstEntry") != testMap1.end());
	EXPECT_TRUE(testMap1.find("SecondEntry") != testMap1.end());
	EXPECT_TRUE(testMap1.find("ThirdEntry") != testMap1.end());
	EXPECT_TRUE(testMap1.find("FourthEntry") != testMap1.end());
	EXPECT_TRUE(testMap1.find("FifthEntry") != testMap1.end());
	EXPECT_TRUE(testMap1.find("SixthEntry") == testMap1.end());

	EXPECT_EQ(0, testMap1.find("FirstEntry")->second);
	EXPECT_EQ(1, testMap1.find("SecondEntry")->second);
	EXPECT_EQ(3, testMap1.find("ThirdEntry")->second);
	EXPECT_EQ(5, testMap1.find("FourthEntry")->second);
	EXPECT_EQ(4, testMap1.find("FifthEntry")->second);
}
