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

#include "i6engine/modules/manager/IDManager.h"

#include "gtest/gtest.h"

TEST(IDManager, getID) {
	i6engine::modules::IDManager idm;

	for (int i = 0; i < 100; ++i) {
		EXPECT_EQ(idm.getID(), i);
	}

	for (int i = 0; i < 100; ++i) {
		EXPECT_EQ(idm.getID(10), 100 + i * 10);
	}

	for (int i = 0; i < 100; ++i) {
		EXPECT_EQ(idm.getID(3), 1100 + i * 3);
	}
}

TEST(IDManager, setBounds) {
	i6engine::modules::IDManager idm;

	idm.setBounds(50, 100);

	for (int i = 0; i <= 50; ++i) {
		EXPECT_EQ(50 + i, idm.getID());
	}

	ASSERT_THROW(idm.getID(), i6engine::utils::exceptions::SystemFailureException);
}
