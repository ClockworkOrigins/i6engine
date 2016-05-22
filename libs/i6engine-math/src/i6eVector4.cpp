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

#include "i6engine/math/i6eVector4.h"

#include "LinearMath/btVector3.h"

#include "OGRE/OgreVector4.h"

namespace i6e {
namespace math {

	const double i6eVector4::EPSILON = 1e-15;

	i6eVector4::i6eVector4(const Ogre::Vector4 & ogre) : _w(ogre.w), _x(ogre.x), _y(ogre.y), _z(ogre.z) {
	}

	i6eVector4::i6eVector4(const btVector4 & bullet) : _w(bullet.getW()), _x(bullet.getX()), _y(bullet.getY()), _z(bullet.getZ()) {
	}

	i6eVector4::i6eVector4(const std::map<std::string, std::string> & params, const std::string & prefix) : _w(), _x(), _y(), _z() {
		std::stringstream ss(params.find(prefix)->second);
		ss >> _w >> _x >> _y >> _z;
	}

	i6eVector4::i6eVector4(const std::string & s) : _w(), _x(), _y(), _z() {
		std::stringstream stream(s);
		stream >> _w >> _x >> _y >> _z;
	}

	void i6eVector4::insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const {
		std::stringstream ss;
		ss << _w << " " << _x << " " << _y << " " << _z;
		map[prefix] = ss.str();
	}

	Ogre::Vector4 i6eVector4::toOgre() const {
		return Ogre::Vector4(_x, _y, _z, _w);
	}

	btVector4 i6eVector4::toBullet() const {
		return btVector4(_x, _y, _z, _w);
	}

	std::string i6eVector4::toString() const {
		std::stringstream ss;
		ss << getW() << " " << getX() << " " << getY() << " " << getZ();
		return ss.str();
	}

	std::ostream & operator<<(std::ostream & stream, const i6eVector4 & v) {
		stream << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ", " << v.getW() << ")";
		return stream;
	}

} /* namespace math */
} /* namespace i6e */
