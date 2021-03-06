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

#include "i6engine/api/manager/IDManager.h"

#include "gtest/gtest.h"

TEST(IDManager, getID) {
	i6e::api::IDManager idm;

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
	i6e::api::IDManager idm;

	idm.setBounds(50, 100);

	for (int i = 0; i <= 50; ++i) {
		EXPECT_EQ(50 + i, idm.getID());
	}

	ASSERT_THROW(idm.getID(), i6e::utils::exceptions::SystemFailureException);
}
