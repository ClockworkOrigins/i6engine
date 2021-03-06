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

#include "i6engine/math/i6eVector.h"

#include "i6engine/utils/Exceptions.h"

#include "LinearMath/btVector3.h"

#include "OGRE/OgreVector2.h"
#include "OGRE/OgreVector3.h"

namespace i6e {
namespace math {

	const double i6eVector::EPSILON = 1e-15;

	const i6eVector i6eVector::ZERO = i6eVector(0.0, 0.0, 0.0);

	i6eVector::i6eVector(const Ogre::Vector3 & ogre) : _x(ogre.x), _y(ogre.y), _z(ogre.z), _valid(true) {
	}

	i6eVector::i6eVector(const btVector3 & bullet) : _x(bullet.getX()), _y(bullet.getY()), _z(bullet.getZ()), _valid(true) {
	}

	i6eVector::i6eVector(const std::string & s) : _x(), _y(), _z(), _valid(true) {
		std::stringstream stream(s);
		stream >> _x >> _y >> _z;
	}

	i6eVector::i6eVector(const std::map<std::string, std::string> & params, const std::string & prefix) : _x(), _y(), _z(), _valid(true) {
		std::stringstream stream(params.find(prefix)->second);
		stream >> _x >> _y >> _z;
	}

	void i6eVector::insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const {
		std::stringstream ss;
		ss << _x << " " << _y << " " << _z;
		map[prefix] = ss.str();
	}

	i6eVector i6eVector::normalize() const {
		if (length() < EPSILON) {
			ISIXE_THROW_API("i6eVector", "Tried to normalize a (0, 0, 0) vector");
		}
		return i6eVector(*this / length());
	}

	bool i6eVector::operator==(const i6eVector & b) const {
		return std::fabs(_x - b.getX()) < EPSILON && std::fabs(_y - b.getY()) < EPSILON && std::fabs(_z - b.getZ()) < EPSILON;
	}

	double i6eVector::crossAngle(const i6eVector & a, const i6eVector & b) {
		if (a.length() < EPSILON || b.length() < EPSILON) {
			return 0;
		}

		double x = std::acos(scalProd(a.normalize(), b.normalize()));
		return x;
	}

	double i6eVector::crossAngleSigned(const i6eVector & a, const i6eVector & b, const i6eVector & n) {
		if (a.length() < EPSILON || b.length() < EPSILON) {
			return 0;
		}

		double sProd = scalProd(a.normalize(), b.normalize());

		if (sProd > 1.0) {
			sProd = 1.0;
		} else if (sProd < -1.0) {
			sProd = -1.0;
		}

		double x = std::acos(sProd);
		i6eVector cross = a * b;
		if (scalProd(n, cross) < 0) {
			x = -x;
		}
		return x;
	}

	double i6eVector::length() const {
		return std::sqrt(_x * _x + _y * _y + _z * _z);
	}

	btVector3 i6eVector::toBullet() const {
		return btVector3(_x, _y, _z);
	}

	Ogre::Vector3 i6eVector::toOgre() const {
		return Ogre::Vector3(_x, _y, _z);
	}

	Ogre::Vector2 i6eVector::toOgre2() const {
		return Ogre::Vector2(_x, _z);
	}

	std::string i6eVector::toString() const {
		std::stringstream s;
		s.precision(100);
		s << _x << " " << _y << " " << _z;
		return s.str();
	}

	bool i6eVector::isValid() const {
		return _valid;
	}

	void i6eVector::setValid(bool b) {
		_valid = b;
	}

	std::ostream & operator<<(std::ostream & stream, const i6eVector & v) {
		stream << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
		return stream;
	}

} /* namespace math */
} /* namespace i6e */
