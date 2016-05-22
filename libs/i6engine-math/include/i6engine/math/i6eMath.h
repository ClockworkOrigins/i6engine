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

/**
 * \addtogroup math
 * @{
 */

#ifndef __I6ENGINE_MATH_I6EMATH_H__
#define __I6ENGINE_MATH_I6EMATH_H__

#include <cmath>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

/**
 * \brief PI
 */
const double PI = 4 * std::atan(1);

namespace i6e {
namespace math {

	/**
	 * \brief returns the distance of a point to a line specified by offset and direction
	 */
	inline double disPointLine(const Vec3 & offset, const Vec3 & direction, const Vec3 & point) {
		if (std::fabs(direction.length()) < 1e-15) {
			ISIXE_THROW_API("disPointLine", "given direction vector is (0, 0, 0)");
		}
		Vec3 c = direction * (point - offset);
		return c.length() / direction.length();
	}

	/**
	 * \brief returns "n over k"
	 * keep in mind that this function is likely to overflow with relatively small values
	 */
	inline uint64_t binom(uint32_t n, uint32_t k) {
		uint64_t bin = 1;
		if (k > n - k) {
			k = n - k;
		}
		for (uint32_t i = 1; i <= k; ++i) {
			bin *= n--;
			bin /= i;
		}
		return bin;
	}

	/**
	 * \brief returns the directional vector
	 */
	inline Vec3 rotateVector(const Vec3 & pos, const Quaternion & rot) {
		return (rot * pos * rot.inverse()).toVector();
	}

} /* namespace math */
} /* namespace i6e */

#endif /* __I6ENGINE_MATH_I6EMATH_H__ */

/**
 * @}
 */
