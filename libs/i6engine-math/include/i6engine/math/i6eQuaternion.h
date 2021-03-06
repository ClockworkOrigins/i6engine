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
 * A class to "transport" Quaternion
 *
 * its better to send only 1 "type" to the game then many
 */

#ifndef  __I6ENGINE_MATH_I6EQUATERNION_H__
#define  __I6ENGINE_MATH_I6EQUATERNION_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace Ogre {
	class Quaternion;
} /* namespace Ogre */
class btQuaternion;

namespace i6e {
namespace math {

	class i6eVector;

	/**
	 * \class i6eQuaternion
	 * \brief Class describing a 3d rotation.
	 *
	 * Descibes a 3d rotation with 4-dimensional vectors. Conversion of vectors from Ogre and Bullet possible.
	 * Implementation identical to i6eVector4.
	 *
	 * Bullet stores X first, Ogre stores W first... not binary compatible.
	 */
	class ISIXE_MATH_API i6eQuaternion {
		static const double EPSILON;

	public:
		static const i6eQuaternion IDENTITY;

		/**
		 * \brief Creates a new vector with all values set to 0.
		 */
		i6eQuaternion() : _w(1.0), _x(0.0), _y(0.0), _z(0.0) {
		}

		/**
		 * \brief Creates a new vector with given values.
		 * \important Be careful with the ordering. i6eQuaternion expects W first, Bullet-Quaternion expects W last
		 */
		i6eQuaternion(const double w, const double x, const double y, const double z) : _w(w), _x(x), _y(y), _z(z) {
		}

		/**
		 * \brief Constructor for easy rotations
		 *
		 * \param[in] axis the axis around which the Object should be rotated
		 * \param[in] angle angle by which the object should be rotated using in radians
		 */
		i6eQuaternion(const i6eVector & axis, const double angle);

		/**
		 * \brief Copy constructor with vector from Ogre.
		 *
		 * \param[in] ogre Vector to copy
		 */
		explicit i6eQuaternion(const Ogre::Quaternion & ogre);

		/**
		 * \brief Copy constructor with vector from Bullet.
		 *
		 * \param[in] bullet Vector to copy
		 */
		explicit i6eQuaternion(const btQuaternion & bullet);

		/*
		 * \brief Constructs a quaternion from an attributeMap
		 *
		 * \param[in] params the attributeMap containing informations for the quaternion
		 */
		i6eQuaternion(const std::map<std::string, std::string> & params, const std::string & prefix);

		/*
		 * \brief Constructs a quaternion from a string
		 */
		explicit i6eQuaternion(const std::string & str);

		/**
		 * \brief Empty
		 *
		 * Nothing to do right now, should free memory
		 */
		~i6eQuaternion() {
		}

		/**
		 * \brief getters for the values of the quaternion
		 */
		double getW() const { return _w; }
		double getX() const { return _x; }
		double getY() const { return _y; }
		double getZ() const { return _z; }

		/**
		 * \brief setters for the values of the quaternion
		 */
		void setW(const double w) { _w = w; }
		void setX(const double x) { _x = x; }
		void setY(const double y) { _y = y; }
		void setZ(const double z) { _z = z; }

		/**
		 * \brief Operator '+' for Quaternions
		 */
		i6eQuaternion operator+(const i6eQuaternion & b) const {
			return i6eQuaternion(_w + b.getW(), _x + b.getX(), _y + b.getY(), _z + b.getZ());
		}

		/**
		 * \brief Operator '-' for Quaternions
		 */
		i6eQuaternion operator-(const i6eQuaternion & b) const {
			return i6eQuaternion(_w - b.getW(), _x - b.getX(), _y - b.getY(), _z - b.getZ());
		}

		/**
		 * \brief Operator '*' for Quaternions
		 */
		i6eQuaternion operator*(const i6eQuaternion & q) const {
	  		return i6eQuaternion(
	  			_w * q.getW() - _x * q.getX() - _y * q.getY() - _z * q.getZ(),
	  			_w * q.getX() + _x * q.getW() + _y * q.getZ() - _z * q.getY(),
	  			_w * q.getY() + _y * q.getW() + _z * q.getX() - _x * q.getZ(),
	  			_w * q.getZ() + _z * q.getW() + _x * q.getY() - _y * q.getX());
		}

		/**
		 * \brief multiplies a quaternion with a scalar
		 */
		i6eQuaternion operator*(const double & v) const {
			return i6eQuaternion(_w * v, _x * v, _y * v, _z * v);
		}

		/**
		 * \brief divides a quaternion by a scalar
		 */
		i6eQuaternion operator/(const double & v) const {
			return i6eQuaternion(_w / v, _x / v, _y / v, _z / v);
		}

		/**
		 * \brief Operator '+=' for Quaternions
		 */
		i6eQuaternion operator+=(const i6eQuaternion & b) {
			setW(getW() + b.getW());
			setX(getX() + b.getX());
			setY(getY() + b.getY());
			setZ(getZ() + b.getZ());
			return *this;
		}

		/**
		 * \brief inserts this quaternion into an attributeMap using the given prefix
		 */
		void insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const;

		/**
		 * \brief return the inverted vector of the quaternion
		 */
		i6eQuaternion inverse() const {
			return i6eQuaternion(_w, -_x, -_y, -_z);
		}

		/**
		 * \brief return the length of the quaternion
		 */
		double length() const;

		/**
		 * \brief normalieses Quaternion to length 1
		 */
		i6eQuaternion normalize() const;

		/**
		 * \brief compares this and q
		 * \param[in] q Quaternion to compare with
		 * \return true if they are similar according to epsi(angle < eps)
		 */
		bool equals(const i6eQuaternion & q, double eps = i6eQuaternion::EPSILON) const;

		/**
		 * \brief converts the quaternion to an angle / axis representation
		 * \param[out] angle rotation by this angle
		 * \param[out] axis around this axis
		 */
		void toAxisAngle(i6eVector & axis, double & angle) const;

		/**
		 * \brief Operator '==' for Quaternions
		 */
		bool operator==(const i6eQuaternion & b) const;

		/**
		 * \brief Operator '!=' for Quaternions
		 */
		bool operator!=(const i6eQuaternion & b) const {
	  		return !(*this == b);
	  	}

	  	/**
		 * \brief creates a 3D vector containing x, y and z
		 * \return
		 */
		i6eVector toVector() const;

		/**
		 * \brief Convert whole vector to Ogre.
		 *
		 * \return Vector converted to Ogre
		 */
		Ogre::Quaternion toOgre() const;

		/**
		 * \brief Convert whole vector to Bullet.
		 *
		 * \return Vector converted to Bullet
		 */
		btQuaternion toBullet() const;

		/**
		 * \brief generates a string containing a readable version of the quaternion
		 */
		std::string toString() const;

		/**
		 * \brief serialize method for quaternion;
		 */
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & _w;
			ar & _x;
			ar & _y;
			ar & _z;
		}

		private:
			double _w, _x, _y, _z;
	};

	/**
	 * \brief stream operator for quaternion
	 */
	ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const i6eQuaternion & q);

	/**
	 * \brief multiplies quaternion with i6eVector
	 */
	ISIXE_MATH_API i6eQuaternion operator*(const i6eQuaternion & q, const i6eVector & w);

	/**
	 * \brief multiplies i6eVector with quaternion
	 */
	ISIXE_MATH_API i6eQuaternion operator*(const i6eVector & w, const i6eQuaternion & q);

	/**
	 * \brief creates dot product out of two quaternions
	 */
	ISIXE_MATH_API double dotProduct(const i6eQuaternion & p, const i6eQuaternion & q);

	/**
	 * \brief calculates scalar product of current Quaternion and p
	 */
	ISIXE_MATH_API double scalProd(const i6eQuaternion & p, const i6eQuaternion & q);

} /* namespace math */
} /* namespace i6e */

typedef i6e::math::i6eQuaternion Quaternion;

#endif /* __I6ENGINE_MATH_I6EQUATERNION_H__ */

/**
 * @}
 */
