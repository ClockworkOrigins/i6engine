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

#include "i6engine/math/i6eVector2.h"

#include "i6engine/utils/Exceptions.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace math {

	template<>
	bool i6eVector2<int32_t>::operator==(const i6eVector2<int32_t> & b) const {
		return std::fabs(double(_x - b.getX())) < EPSILON && std::fabs(double(_y - b.getY())) < EPSILON;
	}

	template<>
	bool i6eVector2<uint32_t>::operator==(const i6eVector2<uint32_t> & b) const {
		return std::fabs(double(_x - b.getX())) < EPSILON && std::fabs(double(_y - b.getY())) < EPSILON;
	}

} /* namespace math */
} /* namespace i6engine */

std::ostream & operator<<(std::ostream & stream, const Vec2 & v) {
	stream << "(" << v.getX() << ", " << v.getY() << ")";
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const Vec2f & v) {
	stream << "(" << v.getX() << ", " << v.getY() << ")";
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const Vec2i & v) {
	stream << "(" << v.getX() << ", " << v.getY() << ")";
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const Vec2ui & v) {
	stream << "(" << v.getX() << ", " << v.getY() << ")";
	return stream;
}
