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

#include "i6engine/math/i6eQuaternion.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eVector.h"

#include "boost/lexical_cast.hpp"

#include "LinearMath/btQuaternion.h"

#include "OGRE/OgreQuaternion.h"

namespace i6e {
namespace math {

	const double i6eQuaternion::EPSILON = 1e-15;

	bool i6eQuaternion::equals(const i6eQuaternion & q, double eps /* = i6eQuaternion::EPSILON*/) const {
		i6eQuaternion diff = this->inverse() * q;
		diff = diff.normalize();
		double angle;
		i6eVector axis;
		diff.toAxisAngle(axis, angle);
		return std::fabs(angle) < eps;
	}

	i6eQuaternion i6eQuaternion::normalize() const {
		double l = length();
		if (l > 1) {
			double w = _w / l;
			double x = _x / l;
			double y = _y / l;
			double z = _z / l;

			return i6eQuaternion(w, x, y, z);
		}

		return *this;
	}

	void i6eQuaternion::toAxisAngle(i6eVector & axis, double & angle) const {
		normalize();
		angle = 2 * std::acos(_w);
		if (std::abs(angle) < DBL_EPSILON) {
			axis = i6eVector(0.0, 1.0, 0.0);
		} else {
			axis = i6eVector(_x / sin(angle / 2.0), _y / sin(angle / 2.0), _z / sin(angle / 2.0));
		}
	}

	const i6eQuaternion i6eQuaternion::IDENTITY = i6eQuaternion(1.0, 0.0, 0.0, 0.0);

	i6eQuaternion::i6eQuaternion(const i6eVector & axis, const double angle) : _w(cos(angle / 2.0)), _x(sin(angle / 2.0) * axis.getX()), _y(sin(angle / 2.0) * axis.getY()), _z(sin(angle / 2.0) * axis.getZ()) {
		if (std::fabs(axis.length() - 1.0) > EPSILON) {
			ISIXE_THROW_API("i6eQuaternion", "rotation needs a normalized vector, but current vector " << axis << " has length " << axis.length());
		}
	}

	i6eQuaternion::i6eQuaternion(const Ogre::Quaternion & ogre) : _w(ogre.w), _x(ogre.x), _y(ogre.y), _z(ogre.z) {
	}

	i6eQuaternion::i6eQuaternion(const btQuaternion & bullet) : _w(bullet.getW()), _x(bullet.getX()), _y(bullet.getY()), _z(bullet.getZ()) {
	}

	i6eQuaternion::i6eQuaternion(const std::map<std::string, std::string> & params, const std::string & prefix) : _w(), _x(), _y(), _z() {
		std::stringstream ss(params.find(prefix)->second);
		ss >> _w >> _x >> _y >> _z;
	}

	i6eQuaternion::i6eQuaternion(const std::string & s) : _w(), _x(), _y(), _z() {
		std::stringstream stream(s);
		stream >> _w >> _x >> _y >> _z;
	}

	i6eQuaternion operator*(const i6eQuaternion & q, const i6eVector & w) {
		return i6eQuaternion(
			-q.getX() * w.getX() - q.getY() * w.getY() - q.getZ() * w.getZ(),
			q.getW() * w.getX() + q.getY() * w.getZ() - q.getZ() * w.getY(),
			q.getW() * w.getY() + q.getZ() * w.getX() - q.getX() * w.getZ(),
			q.getW() * w.getZ() + q.getX() * w.getY() - q.getY() * w.getX());
	}

	i6eQuaternion operator*(const i6eVector & w, const i6eQuaternion & q) {
		return i6eQuaternion(
			-w.getX() * q.getX() - w.getY() * q.getY() - w.getZ() * q.getZ(),
			w.getX() * q.getW() + w.getY() * q.getZ() - w.getZ() * q.getY(),
			w.getY() * q.getW() + w.getZ() * q.getX() - w.getX() * q.getZ(),
			w.getZ() * q.getW() + w.getX() * q.getY() - w.getY() * q.getX());
	}

	std::string i6eQuaternion::toString() const {
		std::stringstream logmsg;
		double alpha = std::acos(_w) * 2.0;
		logmsg << "(" << _x / std::sin(alpha / 2.0) << ", " << _y / std::sin(alpha / 2.0) << ", " << _z / std::sin(alpha / 2.0) << ") - " << alpha * 180.0 /  (4.0 * std::atan(1.0));
		return logmsg.str();
	}

	void i6eQuaternion::insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const {
		std::stringstream ss;
		ss << _w << " " << _x << " " << _y << " " << _z;
		map[prefix] = ss.str();
	}

	double i6eQuaternion::length() const {
		return std::sqrt(_w * _w + _x * _x + _y * _y + _z * _z);
	}

	bool i6eQuaternion::operator==(const i6eQuaternion & b) const {
		return std::abs(_w - b.getW()) < EPSILON && std::abs(_x - b.getX()) < EPSILON && std::abs(_y - b.getY()) < EPSILON && std::abs(_z - b.getZ()) < EPSILON;
	}

	i6eVector i6eQuaternion::toVector() const {
		return i6eVector(_x, _y, _z);
	}

	Ogre::Quaternion i6eQuaternion::toOgre() const {
		return Ogre::Quaternion(_w, _x, _y, _z);
	}

	btQuaternion i6eQuaternion::toBullet() const {
		return btQuaternion(_x, _y, _z, _w);
	}

	std::ostream & operator<<(std::ostream & stream, const i6eQuaternion & q) {
		stream << q.getW() << " " << q.getX() << " " << q.getY() << " " << q.getZ();
		return stream;
	}
	
	double scalProd(const i6eQuaternion & p, const i6eQuaternion & q) {
		return p.getW() * q.getW() + p.getX() * q.getX() + p.getY() * q.getY() + p.getZ() * q.getZ();
	}

	double dotProduct(const i6eQuaternion & p, const i6eQuaternion & q) {
		return p.getW() * q.getW() + p.getX() * q.getX() + p.getY() * q.getY() + p.getZ() * q.getZ();
	}

} /* namespace math */
} /* namespace i6e */
