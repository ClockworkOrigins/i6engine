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

#include "i6engine/utils/i6eString.h"

#include "gtest/gtest.h"

TEST(i6eString, split) {
	std::string testString1 = "";
	std::string testString2 = "Foo;Bar;1337";
	std::string testString3 = "1337;Bar;Foo;";
	std::string testString4 = ";1337;Bar;Foo;";
	std::string testString5 = ";;Bar;;";
	std::string testString6 = "Bar";
	std::string testString7 = ";Bar";

	std::vector<std::string> result1 = i6engine::utils::split(testString1, ";");
	std::vector<std::string> result2 = i6engine::utils::split(testString2, ";");
	std::vector<std::string> result3 = i6engine::utils::split(testString3, ";");
	std::vector<std::string> result4 = i6engine::utils::split(testString4, ";");
	std::vector<std::string> result5 = i6engine::utils::split(testString5, ";");
	std::vector<std::string> result6 = i6engine::utils::split(testString6, ";");
	std::vector<std::string> result7 = i6engine::utils::split(testString7, ";");

	ASSERT_TRUE(result1.empty());
	ASSERT_FALSE(result2.empty());
	ASSERT_FALSE(result3.empty());
	ASSERT_FALSE(result4.empty());
	ASSERT_FALSE(result5.empty());
	ASSERT_FALSE(result6.empty());
	ASSERT_FALSE(result7.empty());

	ASSERT_EQ(3, result2.size());
	ASSERT_EQ(3, result3.size());
	ASSERT_EQ(3, result4.size());
	ASSERT_EQ(1, result5.size());
	ASSERT_EQ(1, result6.size());
	ASSERT_EQ(1, result7.size());

	ASSERT_EQ("Foo", result2[0]);
	ASSERT_EQ("Bar", result2[1]);
	ASSERT_EQ("1337", result2[2]);
	ASSERT_EQ("1337", result3[0]);
	ASSERT_EQ("Bar", result3[1]);
	ASSERT_EQ("Foo", result3[2]);
	ASSERT_EQ("1337", result4[0]);
	ASSERT_EQ("Bar", result4[1]);
	ASSERT_EQ("Foo", result4[2]);
	ASSERT_EQ("Bar", result5[0]);
	ASSERT_EQ("Bar", result6[0]);
	ASSERT_EQ("Bar", result7[0]);
}

TEST(i6eString, to_string_with_precision) {
	double a = 0.0;
	double b = 3.1415;
	EXPECT_EQ("0", i6engine::utils::to_string_with_precision(a, 0));
	EXPECT_EQ("0", i6engine::utils::to_string_with_precision(a, 1));
	EXPECT_EQ("0", i6engine::utils::to_string_with_precision(a, 2));
	EXPECT_EQ("3", i6engine::utils::to_string_with_precision(b, 1));
	EXPECT_EQ("3.1", i6engine::utils::to_string_with_precision(b, 2));
	EXPECT_EQ("3.14", i6engine::utils::to_string_with_precision(b, 3));
	EXPECT_EQ("3.142", i6engine::utils::to_string_with_precision(b, 4));
	EXPECT_EQ("3.1415", i6engine::utils::to_string_with_precision(b, 5));
}
