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

#include "i6engine/math/i6eVector.h"

#include "i6engine/utils/Exceptions.h"

#include "boost/lexical_cast.hpp"

#include "LinearMath/btVector3.h"

#include "OGRE/OgreVector2.h"
#include "OGRE/OgreVector3.h"

namespace i6engine {
namespace math {

	const double i6eVector::EPSILON = 1e-15;

	const i6eVector i6eVector::ZERO = i6eVector(0.0, 0.0, 0.0);

	i6eVector::i6eVector(const Ogre::Vector3 & ogre) : _x(ogre.x), _y(ogre.y), _z(ogre.z), _valid(true) {
	}

	i6eVector::i6eVector(const btVector3 & bullet) : _x(bullet.getX()), _y(bullet.getY()), _z(bullet.getZ()), _valid(true) {
	}

	i6eVector::i6eVector(const std::string & s) : _x(), _y(), _z(), _valid(true) {
		std::stringstream stream(s);
		double dx, dy, dz;
		stream >> dx >> dy >> dz;
		_x = dx;
		_y = dy;
		_z = dz;
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
} /* namespace i6engine */
