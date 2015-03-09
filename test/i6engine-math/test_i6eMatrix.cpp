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

#include "i6engine/math/i6eMatrix.h"

#include "gtest/gtest.h"

typedef i6engine::math::i6eMatrix<float> FloatMatrix;

TEST(i6eMatrix, Constructor) {
	FloatMatrix m1;
	FloatMatrix m2(3, 2);

	EXPECT_EQ(0, m1.getWidth());
	EXPECT_EQ(0, m1.getHeight());
	EXPECT_EQ(2, m2.getWidth());
	EXPECT_EQ(3, m2.getHeight());

	for (unsigned int i = 0; i < 3; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			EXPECT_EQ(m2(i, j), 0.0f);
		}
	}

	m2(0, 0, 1.0f);
	m2(1, 1, 1.0f);

	FloatMatrix m3 = m2;

	for (unsigned int i = 0; i < 3; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			if (i == j) {
				EXPECT_EQ(1.0f, m3(i, j));
			} else {
				EXPECT_EQ(0.0f, m3(i, j));
			}
		}
	}
}

TEST(i6eMatrix, setEntry) {
	FloatMatrix m(3, 2);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	ASSERT_THROW(m(3, 0, 7.0f), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(m(2, 2, 8.0f), i6engine::utils::exceptions::ApiException);

	EXPECT_EQ(1.0f, m(0, 0));
	EXPECT_EQ(2.0f, m(0, 1));
	EXPECT_EQ(3.0f, m(1, 0));
	EXPECT_EQ(4.0f, m(1, 1));
	EXPECT_EQ(5.0f, m(2, 0));
	EXPECT_EQ(6.0f, m(2, 1));

	ASSERT_THROW(m(3, 0), i6engine::utils::exceptions::ApiException);
	ASSERT_THROW(m(2, 2), i6engine::utils::exceptions::ApiException);
}

TEST(i6eMatrix, setZero) {
	FloatMatrix m(3, 2);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	EXPECT_EQ(1.0f, m(0, 0));
	EXPECT_EQ(2.0f, m(0, 1));
	EXPECT_EQ(3.0f, m(1, 0));
	EXPECT_EQ(4.0f, m(1, 1));
	EXPECT_EQ(5.0f, m(2, 0));
	EXPECT_EQ(6.0f, m(2, 1));

	m.setZero();

	for (unsigned int i = 0; i < 3; ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			EXPECT_EQ(0.0f, m(i, j));
		}
	}
}

TEST(i6eMatrix, scalMult) {
	FloatMatrix m(3, 2);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	EXPECT_EQ(1.0f, m(0, 0));
	EXPECT_EQ(2.0f, m(0, 1));
	EXPECT_EQ(3.0f, m(1, 0));
	EXPECT_EQ(4.0f, m(1, 1));
	EXPECT_EQ(5.0f, m(2, 0));
	EXPECT_EQ(6.0f, m(2, 1));

	FloatMatrix m1 = m * 5.0f;

	EXPECT_EQ(1.0f, m(0, 0));
	EXPECT_EQ(2.0f, m(0, 1));
	EXPECT_EQ(3.0f, m(1, 0));
	EXPECT_EQ(4.0f, m(1, 1));
	EXPECT_EQ(5.0f, m(2, 0));
	EXPECT_EQ(6.0f, m(2, 1));

	EXPECT_EQ(5.0f, m1(0, 0));
	EXPECT_EQ(10.0f, m1(0, 1));
	EXPECT_EQ(15.0f, m1(1, 0));
	EXPECT_EQ(20.0f, m1(1, 1));
	EXPECT_EQ(25.0f, m1(2, 0));
	EXPECT_EQ(30.0f, m1(2, 1));

	m1 *= 2.5f;

	EXPECT_EQ(12.5f, m1(0, 0));
	EXPECT_EQ(25.0f, m1(0, 1));
	EXPECT_EQ(37.5f, m1(1, 0));
	EXPECT_EQ(50.0f, m1(1, 1));
	EXPECT_EQ(62.5f, m1(2, 0));
	EXPECT_EQ(75.0f, m1(2, 1));
}

TEST(i6eMatrix, scalDiv) {
	FloatMatrix m(3, 2);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	EXPECT_EQ(1.0f, m(0, 0));
	EXPECT_EQ(2.0f, m(0, 1));
	EXPECT_EQ(3.0f, m(1, 0));
	EXPECT_EQ(4.0f, m(1, 1));
	EXPECT_EQ(5.0f, m(2, 0));
	EXPECT_EQ(6.0f, m(2, 1));

	FloatMatrix m1 = m / 2.0f;

	EXPECT_EQ(1.0f, m(0, 0));
	EXPECT_EQ(2.0f, m(0, 1));
	EXPECT_EQ(3.0f, m(1, 0));
	EXPECT_EQ(4.0f, m(1, 1));
	EXPECT_EQ(5.0f, m(2, 0));
	EXPECT_EQ(6.0f, m(2, 1));

	EXPECT_EQ(0.5f, m1(0, 0));
	EXPECT_EQ(1.0f, m1(0, 1));
	EXPECT_EQ(1.5f, m1(1, 0));
	EXPECT_EQ(2.0f, m1(1, 1));
	EXPECT_EQ(2.5f, m1(2, 0));
	EXPECT_EQ(3.0f, m1(2, 1));

	m1 /= 2.5f;

	EXPECT_EQ(0.2f, m1(0, 0));
	EXPECT_EQ(0.4f, m1(0, 1));
	EXPECT_EQ(0.6f, m1(1, 0));
	EXPECT_EQ(0.8f, m1(1, 1));
	EXPECT_EQ(1.0f, m1(2, 0));
	EXPECT_EQ(1.2f, m1(2, 1));
}

TEST(i6eMatrix, equals) {
	FloatMatrix m(3, 2);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	FloatMatrix m1(3, 2);

	EXPECT_FALSE(m == m1);

	m1 = m;

	EXPECT_TRUE(m == m1);

	FloatMatrix m2(3, 3);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	EXPECT_FALSE(m1 == m2);

	FloatMatrix m3 = m2;

	EXPECT_TRUE(m2 == m3);
}

TEST(i6eMatrix, setIdentity) {
	FloatMatrix m(3, 2);

	m.setIdentity();

	for (unsigned int i = 0; i < m.getHeight(); ++i) {
		for (unsigned int j = 0; j < m.getWidth(); ++j) {
			if (i == j) {
				EXPECT_EQ(1.0f, m(i, j));
			} else {
				EXPECT_EQ(0.0f, m(i, j));
			}
		}
	}

	FloatMatrix m1(3, 3);

	m1.setIdentity();

	for (unsigned int i = 0; i < m.getHeight(); ++i) {
		for (unsigned int j = 0; j < m.getWidth(); ++j) {
			if (i == j) {
				EXPECT_EQ(1.0f, m1(i, j));
			} else {
				EXPECT_EQ(0.0f, m1(i, j));
			}
		}
	}

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, 3.0f);
	m(1, 1, 4.0f);
	m(2, 0, 5.0f);
	m(2, 1, 6.0f);

	m.setIdentity();

	for (unsigned int i = 0; i < m.getHeight(); ++i) {
		for (unsigned int j = 0; j < m.getWidth(); ++j) {
			if (i == j) {
				EXPECT_EQ(1.0f, m(i, j));
			} else {
				EXPECT_EQ(0.0f, m(i, j));
			}
		}
	}
}

TEST(i6eMatrix, calcDeterminant) {
	FloatMatrix m(3, 2);

	ASSERT_THROW(m.calculateDeterminant(), i6engine::utils::exceptions::ApiException);

	FloatMatrix m1(3, 3);

	m1(0, 0, 1.0f);
	m1(0, 1, 3.0f);
	m1(0, 2, 5.0f);
	m1(1, 0, -1.0f);
	m1(1, 1, 2.0f);
	m1(1, 2, 0.0f);
	m1(2, 0, 4.0f);
	m1(2, 1, 2.0f);
	m1(2, 2, -3.0f);

	EXPECT_EQ(-65.0f, m1.calculateDeterminant());

	FloatMatrix m2(2, 2);

	m2(0, 0, 2.0f);
	m2(0, 1, -3.0f);
	m2(1, 0, 5.0f);
	m2(1, 1, 1.0f);

	EXPECT_EQ(17.0f, m2.calculateDeterminant());
}

TEST(i6eMatrix, matrixMult) {
	FloatMatrix m2(2, 3);

	m2(0, 0, 1.0f);
	m2(0, 1, 2.0f);
	m2(0, 2, 3.0f);
	m2(1, 0, 4.0f);
	m2(1, 1, 5.0f);
	m2(1, 2, 6.0f);

	FloatMatrix m3(3, 2);

	m3(0, 0, 6.0f);
	m3(0, 1, -1.0f);
	m3(1, 0, 3.0f);
	m3(1, 1, 2.0f);
	m3(2, 0, 0.0f);
	m3(2, 1, -3.0f);

	FloatMatrix m4(10, 64);

	ASSERT_THROW(m4 * m2, i6engine::utils::exceptions::ApiException);

	FloatMatrix m1 = m2 * m3;

	EXPECT_EQ(12.0f, m1(0, 0));
	EXPECT_EQ(-6.0f, m1(0, 1));
	EXPECT_EQ(39.0f, m1(1, 0));
	EXPECT_EQ(-12.0f, m1(1, 1));
	EXPECT_EQ(1.0f, m2(0, 0));
	EXPECT_EQ(2.0f, m2(0, 1));
	EXPECT_EQ(3.0f, m2(0, 2));
	EXPECT_EQ(4.0f, m2(1, 0));
	EXPECT_EQ(5.0f, m2(1, 1));
	EXPECT_EQ(6.0f, m2(1, 2));
	EXPECT_EQ(6.0f, m3(0, 0));
	EXPECT_EQ(-1.0f, m3(0, 1));
	EXPECT_EQ(3.0f, m3(1, 0));
	EXPECT_EQ(2.0f, m3(1, 1));
	EXPECT_EQ(0.0f, m3(2, 0));
	EXPECT_EQ(-3.0f, m3(2, 1));

	FloatMatrix m5(10, 10);
	FloatMatrix m6(10, 10);

	for (unsigned int i = 0; i < 10; ++i) {
		for (unsigned int j = 0; j < 10; ++j) {
			m5(i, j, 10.0f * i + j + 1);
			m6(i, j, 10.0f * i + j + 1);
		}
	}

	FloatMatrix m7 = m5 * m6;

	EXPECT_EQ(3355.0f, m7(0, 0));
	EXPECT_EQ(3410.0f, m7(0, 1));
	EXPECT_EQ(3465.0f, m7(0, 2));
	EXPECT_EQ(3520.0f, m7(0, 3));
	EXPECT_EQ(3575.0f, m7(0, 4));
	EXPECT_EQ(3630.0f, m7(0, 5));
	EXPECT_EQ(3685.0f, m7(0, 6));
	EXPECT_EQ(3740.0f, m7(0, 7));
	EXPECT_EQ(3795.0f, m7(0, 8));
	EXPECT_EQ(3850.0f, m7(0, 9));
	EXPECT_EQ(7955.0f, m7(1, 0));
	EXPECT_EQ(8110.0f, m7(1, 1));
	EXPECT_EQ(8265.0f, m7(1, 2));
	EXPECT_EQ(8420.0f, m7(1, 3));
	EXPECT_EQ(8575.0f, m7(1, 4));
	EXPECT_EQ(8730.0f, m7(1, 5));
	EXPECT_EQ(8885.0f, m7(1, 6));
	EXPECT_EQ(9040.0f, m7(1, 7));
	EXPECT_EQ(9195.0f, m7(1, 8));
	EXPECT_EQ(9350.0f, m7(1, 9));
	EXPECT_EQ(12555.0f, m7(2, 0));
	EXPECT_EQ(12810.0f, m7(2, 1));
	EXPECT_EQ(13065.0f, m7(2, 2));
	EXPECT_EQ(13320.0f, m7(2, 3));
	EXPECT_EQ(13575.0f, m7(2, 4));
	EXPECT_EQ(13830.0f, m7(2, 5));
	EXPECT_EQ(14085.0f, m7(2, 6));
	EXPECT_EQ(14340.0f, m7(2, 7));
	EXPECT_EQ(14595.0f, m7(2, 8));
	EXPECT_EQ(14850.0f, m7(2, 9));
	EXPECT_EQ(17155.0f, m7(3, 0));
	EXPECT_EQ(17510.0f, m7(3, 1));
	EXPECT_EQ(17865.0f, m7(3, 2));
	EXPECT_EQ(18220.0f, m7(3, 3));
	EXPECT_EQ(18575.0f, m7(3, 4));
	EXPECT_EQ(18930.0f, m7(3, 5));
	EXPECT_EQ(19285.0f, m7(3, 6));
	EXPECT_EQ(19640.0f, m7(3, 7));
	EXPECT_EQ(19995.0f, m7(3, 8));
	EXPECT_EQ(20350.0f, m7(3, 9));
	EXPECT_EQ(21755.0f, m7(4, 0));
	EXPECT_EQ(22210.0f, m7(4, 1));
	EXPECT_EQ(22665.0f, m7(4, 2));
	EXPECT_EQ(23120.0f, m7(4, 3));
	EXPECT_EQ(23575.0f, m7(4, 4));
	EXPECT_EQ(24030.0f, m7(4, 5));
	EXPECT_EQ(24485.0f, m7(4, 6));
	EXPECT_EQ(24940.0f, m7(4, 7));
	EXPECT_EQ(25395.0f, m7(4, 8));
	EXPECT_EQ(25850.0f, m7(4, 9));
	EXPECT_EQ(26355.0f, m7(5, 0));
	EXPECT_EQ(26910.0f, m7(5, 1));
	EXPECT_EQ(27465.0f, m7(5, 2));
	EXPECT_EQ(28020.0f, m7(5, 3));
	EXPECT_EQ(28575.0f, m7(5, 4));
	EXPECT_EQ(29130.0f, m7(5, 5));
	EXPECT_EQ(29685.0f, m7(5, 6));
	EXPECT_EQ(30240.0f, m7(5, 7));
	EXPECT_EQ(30795.0f, m7(5, 8));
	EXPECT_EQ(31350.0f, m7(5, 9));
	EXPECT_EQ(30955.0f, m7(6, 0));
	EXPECT_EQ(31610.0f, m7(6, 1));
	EXPECT_EQ(32265.0f, m7(6, 2));
	EXPECT_EQ(32920.0f, m7(6, 3));
	EXPECT_EQ(33575.0f, m7(6, 4));
	EXPECT_EQ(34230.0f, m7(6, 5));
	EXPECT_EQ(34885.0f, m7(6, 6));
	EXPECT_EQ(35540.0f, m7(6, 7));
	EXPECT_EQ(36195.0f, m7(6, 8));
	EXPECT_EQ(36850.0f, m7(6, 9));
	EXPECT_EQ(35555.0f, m7(7, 0));
	EXPECT_EQ(36310.0f, m7(7, 1));
	EXPECT_EQ(37065.0f, m7(7, 2));
	EXPECT_EQ(37820.0f, m7(7, 3));
	EXPECT_EQ(38575.0f, m7(7, 4));
	EXPECT_EQ(39330.0f, m7(7, 5));
	EXPECT_EQ(40085.0f, m7(7, 6));
	EXPECT_EQ(40840.0f, m7(7, 7));
	EXPECT_EQ(41595.0f, m7(7, 8));
	EXPECT_EQ(42350.0f, m7(7, 9));
	EXPECT_EQ(40155.0f, m7(8, 0));
	EXPECT_EQ(41010.0f, m7(8, 1));
	EXPECT_EQ(41865.0f, m7(8, 2));
	EXPECT_EQ(42720.0f, m7(8, 3));
	EXPECT_EQ(43575.0f, m7(8, 4));
	EXPECT_EQ(44430.0f, m7(8, 5));
	EXPECT_EQ(45285.0f, m7(8, 6));
	EXPECT_EQ(46140.0f, m7(8, 7));
	EXPECT_EQ(46995.0f, m7(8, 8));
	EXPECT_EQ(47850.0f, m7(8, 9));
	EXPECT_EQ(44755.0f, m7(9, 0));
	EXPECT_EQ(45710.0f, m7(9, 1));
	EXPECT_EQ(46665.0f, m7(9, 2));
	EXPECT_EQ(47620.0f, m7(9, 3));
	EXPECT_EQ(48575.0f, m7(9, 4));
	EXPECT_EQ(49530.0f, m7(9, 5));
	EXPECT_EQ(50485.0f, m7(9, 6));
	EXPECT_EQ(51440.0f, m7(9, 7));
	EXPECT_EQ(52395.0f, m7(9, 8));
	EXPECT_EQ(53350.0f, m7(9, 9));
}

TEST(i6eMatrix, matrixAdd) {
	FloatMatrix m1(2, 3);

	m1(0, 0, 1.0f);
	m1(0, 1, -3.0f);
	m1(0, 2, 2.0f);
	m1(1, 0, 1.0f);
	m1(1, 1, 2.0f);
	m1(1, 2, 7.0f);

	FloatMatrix m2(2, 3);

	m2(0, 0, 0.0f);
	m2(0, 1, 3.0f);
	m2(0, 2, 5.0f);
	m2(1, 0, 2.0f);
	m2(1, 1, 1.0f);
	m2(1, 2, -1.0f);

	FloatMatrix m3(10, 64);

	ASSERT_THROW(m3 + m2, i6engine::utils::exceptions::ApiException);

	FloatMatrix m4 = m1 + m2;

	EXPECT_EQ(1.0f, m4(0, 0));
	EXPECT_EQ(0.0f, m4(0, 1));
	EXPECT_EQ(7.0f, m4(0, 2));
	EXPECT_EQ(3.0f, m4(1, 0));
	EXPECT_EQ(3.0f, m4(1, 1));
	EXPECT_EQ(6.0f, m4(1, 2));
}

TEST(i6eMatrix, matrixSub) {
	FloatMatrix m1(2, 3);

	m1(0, 0, 1.0f);
	m1(0, 1, -3.0f);
	m1(0, 2, 2.0f);
	m1(1, 0, 1.0f);
	m1(1, 1, 2.0f);
	m1(1, 2, 7.0f);

	FloatMatrix m2(2, 3);

	m2(0, 0, 0.0f);
	m2(0, 1, 3.0f);
	m2(0, 2, 5.0f);
	m2(1, 0, 2.0f);
	m2(1, 1, 1.0f);
	m2(1, 2, -1.0f);

	FloatMatrix m3(10, 64);

	ASSERT_THROW(m3 + m2, i6engine::utils::exceptions::ApiException);

	FloatMatrix m4 = m1 + m2;
	m4 -= m2;

	EXPECT_EQ(1.0f, m4(0, 0));
	EXPECT_EQ(-3.0f, m4(0, 1));
	EXPECT_EQ(2.0f, m4(0, 2));
	EXPECT_EQ(1.0f, m4(1, 0));
	EXPECT_EQ(2.0f, m4(1, 1));
	EXPECT_EQ(7.0f, m4(1, 2));
}

TEST(i6eMatrix, pow) {
	FloatMatrix m5(10, 10);

	for (unsigned int i = 0; i < 10; ++i) {
		for (unsigned int j = 0; j < 10; ++j) {
			m5(i, j, 10.0f * i + j + 1);
		}
	}

	FloatMatrix m7 = FloatMatrix::pow(m5, 2);

	EXPECT_EQ(3355.0f, m7(0, 0));
	EXPECT_EQ(3410.0f, m7(0, 1));
	EXPECT_EQ(3465.0f, m7(0, 2));
	EXPECT_EQ(3520.0f, m7(0, 3));
	EXPECT_EQ(3575.0f, m7(0, 4));
	EXPECT_EQ(3630.0f, m7(0, 5));
	EXPECT_EQ(3685.0f, m7(0, 6));
	EXPECT_EQ(3740.0f, m7(0, 7));
	EXPECT_EQ(3795.0f, m7(0, 8));
	EXPECT_EQ(3850.0f, m7(0, 9));
	EXPECT_EQ(7955.0f, m7(1, 0));
	EXPECT_EQ(8110.0f, m7(1, 1));
	EXPECT_EQ(8265.0f, m7(1, 2));
	EXPECT_EQ(8420.0f, m7(1, 3));
	EXPECT_EQ(8575.0f, m7(1, 4));
	EXPECT_EQ(8730.0f, m7(1, 5));
	EXPECT_EQ(8885.0f, m7(1, 6));
	EXPECT_EQ(9040.0f, m7(1, 7));
	EXPECT_EQ(9195.0f, m7(1, 8));
	EXPECT_EQ(9350.0f, m7(1, 9));
	EXPECT_EQ(12555.0f, m7(2, 0));
	EXPECT_EQ(12810.0f, m7(2, 1));
	EXPECT_EQ(13065.0f, m7(2, 2));
	EXPECT_EQ(13320.0f, m7(2, 3));
	EXPECT_EQ(13575.0f, m7(2, 4));
	EXPECT_EQ(13830.0f, m7(2, 5));
	EXPECT_EQ(14085.0f, m7(2, 6));
	EXPECT_EQ(14340.0f, m7(2, 7));
	EXPECT_EQ(14595.0f, m7(2, 8));
	EXPECT_EQ(14850.0f, m7(2, 9));
	EXPECT_EQ(17155.0f, m7(3, 0));
	EXPECT_EQ(17510.0f, m7(3, 1));
	EXPECT_EQ(17865.0f, m7(3, 2));
	EXPECT_EQ(18220.0f, m7(3, 3));
	EXPECT_EQ(18575.0f, m7(3, 4));
	EXPECT_EQ(18930.0f, m7(3, 5));
	EXPECT_EQ(19285.0f, m7(3, 6));
	EXPECT_EQ(19640.0f, m7(3, 7));
	EXPECT_EQ(19995.0f, m7(3, 8));
	EXPECT_EQ(20350.0f, m7(3, 9));
	EXPECT_EQ(21755.0f, m7(4, 0));
	EXPECT_EQ(22210.0f, m7(4, 1));
	EXPECT_EQ(22665.0f, m7(4, 2));
	EXPECT_EQ(23120.0f, m7(4, 3));
	EXPECT_EQ(23575.0f, m7(4, 4));
	EXPECT_EQ(24030.0f, m7(4, 5));
	EXPECT_EQ(24485.0f, m7(4, 6));
	EXPECT_EQ(24940.0f, m7(4, 7));
	EXPECT_EQ(25395.0f, m7(4, 8));
	EXPECT_EQ(25850.0f, m7(4, 9));
	EXPECT_EQ(26355.0f, m7(5, 0));
	EXPECT_EQ(26910.0f, m7(5, 1));
	EXPECT_EQ(27465.0f, m7(5, 2));
	EXPECT_EQ(28020.0f, m7(5, 3));
	EXPECT_EQ(28575.0f, m7(5, 4));
	EXPECT_EQ(29130.0f, m7(5, 5));
	EXPECT_EQ(29685.0f, m7(5, 6));
	EXPECT_EQ(30240.0f, m7(5, 7));
	EXPECT_EQ(30795.0f, m7(5, 8));
	EXPECT_EQ(31350.0f, m7(5, 9));
	EXPECT_EQ(30955.0f, m7(6, 0));
	EXPECT_EQ(31610.0f, m7(6, 1));
	EXPECT_EQ(32265.0f, m7(6, 2));
	EXPECT_EQ(32920.0f, m7(6, 3));
	EXPECT_EQ(33575.0f, m7(6, 4));
	EXPECT_EQ(34230.0f, m7(6, 5));
	EXPECT_EQ(34885.0f, m7(6, 6));
	EXPECT_EQ(35540.0f, m7(6, 7));
	EXPECT_EQ(36195.0f, m7(6, 8));
	EXPECT_EQ(36850.0f, m7(6, 9));
	EXPECT_EQ(35555.0f, m7(7, 0));
	EXPECT_EQ(36310.0f, m7(7, 1));
	EXPECT_EQ(37065.0f, m7(7, 2));
	EXPECT_EQ(37820.0f, m7(7, 3));
	EXPECT_EQ(38575.0f, m7(7, 4));
	EXPECT_EQ(39330.0f, m7(7, 5));
	EXPECT_EQ(40085.0f, m7(7, 6));
	EXPECT_EQ(40840.0f, m7(7, 7));
	EXPECT_EQ(41595.0f, m7(7, 8));
	EXPECT_EQ(42350.0f, m7(7, 9));
	EXPECT_EQ(40155.0f, m7(8, 0));
	EXPECT_EQ(41010.0f, m7(8, 1));
	EXPECT_EQ(41865.0f, m7(8, 2));
	EXPECT_EQ(42720.0f, m7(8, 3));
	EXPECT_EQ(43575.0f, m7(8, 4));
	EXPECT_EQ(44430.0f, m7(8, 5));
	EXPECT_EQ(45285.0f, m7(8, 6));
	EXPECT_EQ(46140.0f, m7(8, 7));
	EXPECT_EQ(46995.0f, m7(8, 8));
	EXPECT_EQ(47850.0f, m7(8, 9));
	EXPECT_EQ(44755.0f, m7(9, 0));
	EXPECT_EQ(45710.0f, m7(9, 1));
	EXPECT_EQ(46665.0f, m7(9, 2));
	EXPECT_EQ(47620.0f, m7(9, 3));
	EXPECT_EQ(48575.0f, m7(9, 4));
	EXPECT_EQ(49530.0f, m7(9, 5));
	EXPECT_EQ(50485.0f, m7(9, 6));
	EXPECT_EQ(51440.0f, m7(9, 7));
	EXPECT_EQ(52395.0f, m7(9, 8));
	EXPECT_EQ(53350.0f, m7(9, 9));
}

TEST(i6eMatrix, transpose) {
	FloatMatrix m1(2, 3);

	m1(0, 0, 1.0f);
	m1(0, 1, 8.0f);
	m1(0, 2, -3.0f);
	m1(1, 0, 4.0f);
	m1(1, 1, -2.0f);
	m1(1, 2, 5.0f);

	FloatMatrix m2 = FloatMatrix::transpose(m1);

	EXPECT_EQ(1.0f, m2(0, 0));
	EXPECT_EQ(4.0f, m2(0, 1));
	EXPECT_EQ(8.0f, m2(1, 0));
	EXPECT_EQ(-2.0f, m2(1, 1));
	EXPECT_EQ(-3.0f, m2(2, 0));
	EXPECT_EQ(5.0f, m2(2, 1));
}

TEST(i6eMatrix, invert) {
	FloatMatrix m1(2, 3);

	m1(0, 0, 1.0f);
	m1(0, 1, 8.0f);
	m1(0, 2, -3.0f);
	m1(1, 0, 4.0f);
	m1(1, 1, -2.0f);
	m1(1, 2, 5.0f);

	ASSERT_THROW(FloatMatrix::invert(m1), i6engine::utils::exceptions::ApiException);

	FloatMatrix m2(3, 3);

	m2(0, 0, 1.0f);
	m2(0, 1, 2.0f);
	m2(0, 2, 0.0f);
	m2(1, 0, 2.0f);
	m2(1, 1, 3.0f);
	m2(1, 2, 0.0f);
	m2(2, 0, 3.0f);
	m2(2, 1, 4.0f);
	m2(2, 2, 1.0f);

	FloatMatrix m3 = FloatMatrix::invert(m2);

	FloatMatrix m4(3, 3);

	m4(0, 0, 1.0f);
	m4(0, 1, 0.0f);
	m4(0, 2, 2.0f);
	m4(1, 0, 2.0f);
	m4(1, 1, 1.0f);
	m4(1, 2, 3.0f);
	m4(2, 0, 0.0f);
	m4(2, 1, 3.0f);
	m4(2, 2, 1.0f);

	FloatMatrix m5 = FloatMatrix::invert(m4);

	EXPECT_EQ(-3.0f, m3(0, 0));
	EXPECT_EQ(2.0f, m3(0, 1));
	EXPECT_EQ(0.0f, m3(0, 2));
	EXPECT_EQ(2.0f, m3(1, 0));
	EXPECT_EQ(-1.0f, m3(1, 1));
	EXPECT_EQ(0.0f, m3(1, 2));
	EXPECT_EQ(1.0f, m3(2, 0));
	EXPECT_EQ(-2.0f, m3(2, 1));
	EXPECT_EQ(1.0f, m3(2, 2));

	EXPECT_EQ(-2.0f, m5(0, 0));
	EXPECT_EQ(6.0f / 4.0f, m5(0, 1));
	EXPECT_EQ(-0.5f, m5(0, 2));
	EXPECT_EQ(-0.5f, m5(1, 0));
	EXPECT_EQ(0.25f, m5(1, 1));
	EXPECT_EQ(0.25f, m5(1, 2));
	EXPECT_EQ(6.0f / 4.0f, m5(2, 0));
	EXPECT_EQ(-3.0f / 4.0f, m5(2, 1));
	EXPECT_EQ(0.25f, m5(2, 2));
}

TEST(i6eMatrix, solveSystem) {
	FloatMatrix m1(4, 4);

	m1(0, 0, 2.0f);
	m1(0, 1, -1.0f);
	m1(0, 2, 2.0f);
	m1(0, 3, -3.0f);
	m1(1, 0, -5.0f);
	m1(1, 1, -5.0f);
	m1(1, 2, 3.0f);
	m1(1, 3, -5.0f);
	m1(2, 0, 4.0f);
	m1(2, 1, 4.0f);
	m1(2, 2, -3.0f);
	m1(2, 3, 2.0f);
	m1(3, 0, -5.0f);
	m1(3, 1, 0.0f);
	m1(3, 2, -1.0f);
	m1(3, 3, 3.0f);

	FloatMatrix b(4, 1);

	b(0, 0, 4.0f);
	b(1, 0, 12.0f);
	b(2, 0, -9.0f);
	b(3, 0, 7.0f);

	FloatMatrix x(4, 1);

	m1.solveSystem(b, x);

	EXPECT_FLOAT_EQ(-177.0f / 43.0f, x(0, 0));
	EXPECT_FLOAT_EQ(345.0f / 43.0f, x(1, 0));
	EXPECT_FLOAT_EQ(287.0f / 43.0f, x(2, 0));
	EXPECT_FLOAT_EQ(-99.0f / 43.0f, x(3, 0));
}

TEST(i6eMatrix, unarSub) {
	FloatMatrix m(2, 2);

	m(0, 0, 1.0f);
	m(0, 1, 2.0f);
	m(1, 0, -3.0f);
	m(1, 1, 4.0f);

	m = -m;

	EXPECT_FLOAT_EQ(-1.0f, m(0, 0));
	EXPECT_FLOAT_EQ(-2.0f, m(0, 1));
	EXPECT_FLOAT_EQ(3.0f, m(1, 0));
	EXPECT_FLOAT_EQ(-4.0f, m(1, 1));
}
