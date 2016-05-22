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

/*
 * A class to "transport" 3D vector
 *
 * for the math stuff use the vector of the lib (bullet, ogre...)
 *
 * its better to send only 1 "type" to the game then many
 */

#ifndef __I6ENGINE_MATH_I6EVECTOR4_H__
#define __I6ENGINE_MATH_I6EVECTOR4_H__

#include <cmath>
#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace Ogre {
	class Vector4;
} /* namespace Ogre */
class btVector4;

namespace i6e {
namespace math {

	/**
	 * \class i6eVector4
	 * \brief Implements 4-dimensional vectors
	 *
	 * Implements 4-dimensional vectors. Conversion of vectors from Ogre and Bullet possible.
	 */
	class ISIXE_MATH_API i6eVector4 {
		static const double EPSILON;

	public:
		/**
		 * \brief Creates a new vector with all values set to 0.
		 */
		i6eVector4() : _w(0.0), _x(0.0), _y(0.0), _z(0.0) {
		}

		/**
		 * \brief Creates a new vector with all values set to the given values.
		 */
		i6eVector4(const double w, const double x, const double y, const double z) : _w(w), _x(x), _y(y), _z(z) {
		}

		/**
		 * \brief Copy constructor with vector from Ogre.
		 *
		 * \param[in] ogre Vector to copy
		 */
		explicit i6eVector4(const Ogre::Vector4 & ogre);

		/**
		 * \brief Copy constructor with vector from Bullet.
		 *
		 * \param[in] bullet Vector to copy
		 */
		explicit i6eVector4(const btVector4 & bullet);

		/*
		 * \brief Constructs a vector from an attributeMap
		 *
		 * \param[in] params the attributeMap containing informations for the vector
		 * \param[in] prefix prefix of the values
		 */
		i6eVector4(const std::map<std::string, std::string> & params, const std::string & prefix);

		/*
		 * \brief Constructs a Vec4 from a string
		 */
		explicit i6eVector4(const std::string & str);

		/**
		 * \brief Empty
		 *
		 * Nothing to do right now, should free memory
		 */
		~i6eVector4() {
		}

		/**
		 * \brief getters for the values of the Vector
		 */
		double getW() const {
			return _w;
		}
		double getX() const {
			return _x;
		}
		double getY() const {
			return _y;
		}
		double getZ() const {
			return _z;
		}

		/**
		 * \brief setters for the values of the Vector
		 */
		void setW(const double w) {
			_w = w;
		}
		void setX(const double x) {
			_x = x;
		}
		void setY(const double y) {
			_y = y;
		}
		void setZ(const double z) {
			_z = z;
		}

		/**
		 * \brief + operator
		 */
		i6eVector4 operator+(const i6eVector4 & other) const {
			return i6eVector4(_w + other._w, _x + other._x, _y + other._y, _z + other._z);
		}

		/**
		 * \brief - operator
		 */
		i6eVector4 operator-(const i6eVector4 & other) const {
			return i6eVector4(_w - other._w, _x - other._x, _y - other._y, _z - other._z);
		}

		/**
		 * \brief * operator
		 */
		i6eVector4 operator*(double d) const {
			return i6eVector4(_w * d, _x * d, _y * d, _z * d);
		}

		/**
		 * \brief Operator '==' for Vector4
		 */
		bool operator==(const i6eVector4 & b) const {
			return std::abs(_x - b.getX()) < EPSILON && std::abs(_y - b.getY()) < EPSILON && std::abs(_z - b.getZ()) < EPSILON && std::abs(_w - b.getW()) < EPSILON;
		}

		/**
		 * \brief Operator '!=' for Vector4 calls == and negatiates result
		 */
		bool operator!=(const i6eVector4 & other) const {
			return !(*this == other);
		}

		/**
		 * \brief Convert whole vector to Ogre.
		 *
		 * \return Vector converted to Ogre
		 */
		Ogre::Vector4 toOgre() const;

		/**
		 * \brief Convert whole vector to Bullet.
		 *
		 * \return Vector converted to Bullet
		 */
		btVector4 toBullet() const;

		/**
		 * \brief inserts this vector into an attributeMap using the given prefix
		 */
		void insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const;

		/**
		 * \brief returns string representation of Vec4
		 */
		std::string toString() const;

		/**
		 * \brief serializer for the vector
		 */
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & _x;
			ar & _y;
			ar & _z;
			ar & _w;
		}

	private:
		double _w, _x, _y, _z;
	};

	/**
	 * \brief stream operator for the i6eVector4
	 */
	ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const i6eVector4 & v);

} /* namespace math */
} /* namespace i6e */

typedef i6e::math::i6eVector4 Vec4;

#endif /* __I6ENGINE_MATH_I6EVECTOR4_H__ */

/**
 * @}
 */
