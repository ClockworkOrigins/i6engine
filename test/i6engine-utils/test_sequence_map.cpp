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
