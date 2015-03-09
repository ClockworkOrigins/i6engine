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

#include "i6engine/math/i6eVector4.h"

#include "boost/lexical_cast.hpp"

#include "LinearMath/btVector3.h"

#include "OGRE/OgreVector4.h"

namespace i6engine {
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

	void i6eVector4::insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) {
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
} /* namespace i6engine */
