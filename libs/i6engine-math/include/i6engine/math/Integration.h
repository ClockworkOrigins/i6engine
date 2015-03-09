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

#ifndef __I6ENGINE_MATH_INTEGRATION_H__
#define __I6ENGINE_MATH_INTEGRATION_H__

#include <cmath>

#include "i6engine/utils/Exceptions.h"

namespace i6engine {
namespace math {

	/**
	 * \brief integration calculated using trapezoidal rule
	 */
	inline double integrate(double a, double b, uint32_t N, const std::function<double(double)> & f) {
		double h = (b - a) / N;              // step size
		double sum = 0.5 * (f(a) + f(b));    // area
		for (uint32_t i = 1; i < N; i++) {
			double x = a + h * i;
			sum = sum + f(x);
		}
		return sum * h;
	}

} /* namespace math */
} /* namespace i6engine */

#endif /* __I6ENGINE_MATH_INTEGRATION_H__ */

/**
 * @}
 */
