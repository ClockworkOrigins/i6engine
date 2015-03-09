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

namespace i6engine {
namespace math {

	/**
	 * \brief returns the distance of a point to a line
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MATH_I6EMATH_H__ */

/**
 * @}
 */
