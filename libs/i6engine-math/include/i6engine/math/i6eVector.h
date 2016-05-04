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
 * \addtogroup math
 * @{
 */

#ifndef __I6ENGINE_MATH_I6EVECTOR_H__
#define __I6ENGINE_MATH_I6EVECTOR_H__

#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace Ogre {
	class Vector2;
	class Vector3;
} /* namespace Ogre */
class btVector3;

namespace i6e {
namespace math {

	/**
	 * \class i6eVector
	 * \brief Implements 3-dimensional vectors
	 *
	 * Implements 3-dimensional vectors. Conversion of vectors from Ogre and Bullet possible.
	 */
	class ISIXE_MATH_API i6eVector {
		static const double EPSILON;

	public:
		static const i6eVector ZERO;

		/**
		 * \brief Creates a new vector with all values set to 0.
		 */
		i6eVector() : _x(0.0), _y(0.0), _z(0.0), _valid(false) {
		}

		/**
		 * \brief Creates a new vector with all values set to the given values
		 */
		i6eVector(const double x, const double y, const double z) : _x(x), _y(y), _z(z), _valid(true) {
		}

		/**
		 * \brief Copy constructor with vector from Ogre.
		 *
		 * \param[in] ogre Vector to copy
		 */
		explicit i6eVector(const Ogre::Vector3 & ogre);

		/**
		 * \brief Copy constructor with vector from Bullet.
		 *
		 * \param[in] bullet Vector to copy
		 */
		explicit i6eVector(const btVector3 & bullet);

		/*
		 * \brief Constructs a vector from a std::string
		 *
		 * is std::string matches the format "x y z"
		 */
		explicit i6eVector(const std::string & s);

		/*
		 * \brief Constructs a vector from an attributeMap
		 *
		 * \param[in] params the attributeMap containing informations for the vector
		 * \param[in] prefix prefix of the values
		 */
		i6eVector(const std::map<std::string, std::string> & params, const std::string & prefix);

		/**
		 * \brief destructor
		 */
		~i6eVector() {
		}

		/**
		 * \brief getters for the values of the Vector
		 */
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
		 * \brief Operator '+' for Vectors
		 */
		i6eVector operator+(const i6eVector & b) const {
			return i6eVector(getX() + b.getX(), getY() + b.getY(), getZ() + b.getZ());
		}

		/**
		 * \brief Operator '-' for Vectors
		 */
		i6eVector operator-(const i6eVector & b) const {
			return i6eVector(getX() - b.getX(), getY() - b.getY(), getZ() - b.getZ());
		}

		/**
		 * \brief operator '*' for vectors
		 */
		i6eVector operator*(const i6eVector & b) const {
			return i6eVector(_y * b._z - _z * b._y, _z * b._x - _x * b._z, _x * b._y - _y * b._x);
		}

		/**
		 * \brief operator '*' for scalars
		 */
		i6eVector operator*(const double b) const {
			return i6eVector(_x * b, _y * b, _z * b);
		}

		/**
		 * \brief operator '*' for vectors
		 */
		static double scalProd(const i6eVector & a, const i6eVector & b) {
			return a._x * b._x + a._y * b._y + a._z * b._z;
		}

		/**
		 * \brief Operator '/' for Vectors with a scalar
		 */
		i6eVector operator/(const double b) const {
			return i6eVector(getX() / b, getY() / b, getZ() / b);
		}

		/**
		 * \brief Operator '+=' for Vectors
		 */
		i6eVector operator+=(const i6eVector & b) {
			setX(getX() + b.getX());
			setY(getY() + b.getY());
			setZ(getZ() + b.getZ());
			return *this;
		}

		/**
		 * \brief Operator '-=' for Vectors
		 */
		i6eVector operator-=(const i6eVector & b) {
			setX(_x - b.getX());
			setY(_y - b.getY());
			setZ(_z - b.getZ());
			return *this;
		}

		/**
		 * \brief Operator '==' for Vector
		 */
		bool operator==(const i6eVector & b) const;

		/**
		 * \brief Operator '!=' for Vector
		 */
		bool operator!=(const i6eVector & b) const {
			return !(*this == b);
		}

		/**
		 * \brief returns the normalized Vector
		 */
		i6eVector normalize() const;

		/**
		 * \brief Operator '*' for Vectors (cross product)
		 */
		static i6eVector crossProd(const i6eVector & a, const i6eVector & b) {
			 return i6eVector(a._y * b._z - a._z * b._y, a._z * b._x - a._x * b._z, a._x * b._y - a._y * b._x);
		}

		/**
		 * \brief angle between two vectors
		 * \param[in] a first vector
		 * \param[in] b second vector
		 * \returns angle as a value between [0; PI]
		 */
		static double crossAngle(const i6eVector & a, const i6eVector & b);

		/**
		 * \brief signed angle between two vectors
		 * \param[in] a starting vector
		 * \param[in] b end vector
		 * \param[in] n normal vector of plan containing a and b
		 * \returns angle as a value between ]-PI; PI]
		 */
		static double crossAngleSigned(const i6eVector & a, const i6eVector & b, const i6eVector & n);

		/**
		 * \brief inserts this vector into an attributeMap using the given prefix
		 */
		void insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const;

		/**
		 * \brief muliplies two Vectors per component
		 * \param b
		 */
		void mulComponents(const i6eVector & b) {
			_x = _x * b.getX();
			_y = _y * b.getY();
			_z = _z * b.getZ();
		}

		/**
		 * \brief determins whether the vector contains valid data
		 * This has to be set by the user. Operations resulting in components becoming
		 * NaN or Infinity will NOT change this flag
		 */
		bool isValid() const;
		void setValid(bool b);

		/**
		 * \brief length of the Vector
		 */
		double length() const;

		/**
		 * \brief Convert whole vector to Ogre.
		 *
		 * \return Vector converted to Ogre
		 */
		Ogre::Vector3 toOgre() const;

		/**
		 * \brief Convert x and z values to a 2-dimensional vector in Ogre.
		 *
		 * \return 2-dimensional vector converted to Ogre
		 */
		Ogre::Vector2 toOgre2() const;

		/**
		 * \brief Convert whole vector to Bullet.
		 *
		 * \return Vector converted to Bullet
		 */
		btVector3 toBullet() const;

		/**
		* \brief converts Vector to std::string
		*
		* converts the vector to a std::string using the Format "x y z"
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
		}

	private:
		double _x, _y, _z;
		bool _valid;
	};

	/**
	 * \brief stream operator for the i6eVector
	 */
	ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const i6eVector & v);

} /* namespace math */
} /* namespace i6e */

typedef i6e::math::i6eVector Vec3;

#endif /* __I6ENGINE_MATH_I6EVECTOR_H__ */

/**
 * @}
 */
