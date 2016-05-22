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
