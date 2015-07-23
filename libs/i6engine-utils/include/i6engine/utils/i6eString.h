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

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_I6ESTRING_H__ */

/**
 * @}
 */
