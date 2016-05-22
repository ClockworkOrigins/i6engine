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

#include "i6engine/math/i6eVector2.h"

#include "i6engine/utils/Exceptions.h"

#include "boost/lexical_cast.hpp"

namespace i6e {
namespace math {

	/*template<>
	bool i6eVector2<int32_t>::operator==(const i6eVector2<int32_t> & b) const {
		return std::fabs(double(_x - b.getX())) < EPSILON && std::fabs(double(_y - b.getY())) < EPSILON;
	}

	template<>
	bool i6eVector2<uint32_t>::operator==(const i6eVector2<uint32_t> & b) const {
		return std::fabs(double(_x - b.getX())) < EPSILON && std::fabs(double(_y - b.getY())) < EPSILON;
	}*/

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
