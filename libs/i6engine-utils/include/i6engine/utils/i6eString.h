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
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_I6ESTRING_H__
#define __I6ENGINE_UTILS_I6ESTRING_H__

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace utils {

	/**
	 * \brief returns a vector of strings separated by the given delimitter
	 * e.g. a;b;c with delimitter ; will return a vector containing a, b and c
	 */
	inline std::vector<std::string> split(const std::string & str, const std::string & delim) {
		std::vector<std::string> ret;

		size_t n = 0;
		size_t n2 = str.find(delim);

		while (n2 != std::string::npos) {
			std::string s = str.substr(n, n2 - n);
			n = n2 + 1;
			n2 = str.find(delim, n);

			if (!s.empty()) {
				ret.push_back(s);
			}
		}

		if (str.size() - n > 0) {
			ret.push_back(str.substr(n, str.size() - n));
		}

		return ret;
	}

	/**
	 * \brief returns a string from a floating point value with given precision
	 */
	template<typename T>
	typename std::enable_if<std::is_floating_point<T>::value, std::string>::type to_string_with_precision(const T a_value, const int n) {
		std::ostringstream out;
		out << std::setprecision(n) << a_value;
		return out.str();
	}

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_I6ESTRING_H__ */

/**
 * @}
 */
