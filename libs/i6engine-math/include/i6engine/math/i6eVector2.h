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

#ifndef __I6ENGINE_MATH_I6EVECTOR2_H__
#define __I6ENGINE_MATH_I6EVECTOR2_H__

#include <cfloat>
#include <cmath>
#include <cstdint>
#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Exceptions.h"

namespace i6e {
namespace math {

	/**
	 * \class i6eVector2
	 * \brief Implements 2-dimensional vectors
	 *
	 * Implements 2-dimensional vectors.
	 */
	template<typename T>
	class i6eVector2 {
		static const double EPSILON;

	public:
		static const i6eVector2 ZERO;

		/**
		 * \brief Creates a new vector with all values set to 0.
		 */
		i6eVector2() : _x(T()), _y(T()), _valid(false) {
		}

		/**
		 * \brief Creates a new vector with all values set to the given values
		 */
		i6eVector2(const T x, const T y) : _x(x), _y(y), _valid(true) {
		}

		/*
		 * \brief Constructs a vector from a std::string
		 *
		 * is std::string matches the format "x y"
		 */
		explicit i6eVector2(const std::string & s) : _x(), _y(), _valid(true) {
			std::stringstream stream(s);
			stream >> _x >> _y;
		}

		/*
		 * \brief Constructs a vector from an attributeMap
		 *
		 * \param[in] params the attributeMap containing informations for the vector
		 * \param[in] prefix prefix of the values
		 */
		i6eVector2(const std::map<std::string, std::string> & params, const std::string & prefix) : _x(), _y(), _valid(true) {
			std::stringstream stream(params.find(prefix)->second);
			stream >> _x >> _y;
		}

		/**
		 * \brief destructor
		 */
		~i6eVector2() {
		}

		/**
		 * \brief getters for the values of the Vector
		 */
		T getX() const {
			return _x;
		}
		T getY() const {
			return _y;
		}

		/**
		 * \brief setters for the values of the Vector
		 */
		void setX(const T x) {
			_x = x;
		}
		void setY(const T y) {
			_y = y;
		}

		/**
		 * \brief Operator '+' for Vectors
		 */
		i6eVector2 operator+(const i6eVector2 & b) const {
			return i6eVector2(getX() + b.getX(), getY() + b.getY());
		}

		/**
		 * \brief Operator '-' for Vectors
		 */
		i6eVector2 operator-(const i6eVector2 & b) const {
			return i6eVector2(getX() - b.getX(), getY() - b.getY());
		}

		/**
		 * \brief operator '*' for scalars
		 */
		i6eVector2 operator*(const T b) const {
			return i6eVector2(_x * b, _y * b);
		}

		/**
		 * \brief operator '*=' for scalars
		 */
		i6eVector2 operator*=(T d) {
			_x *= d;
			_y *= d;
			return *this;
		}

		/**
		 * \brief operator '*' for vectors
		 */
		static T scalProd(const i6eVector2 & a, const i6eVector2 & b) {
			return a._x * b._x + a._y * b._y;
		}

		/**
		 * \brief Operator '/' for Vectors with a scalar
		 */
		i6eVector2 operator/(const T b) const {
			return i6eVector2(getX() / b, getY() / b);
		}

		/**
		 * \brief Operator '+=' for Vectors
		 */
		i6eVector2 operator+=(const i6eVector2 & b) {
			setX(getX() + b.getX());
			setY(getY() + b.getY());
			return *this;
		}

		/**
		 * \brief Operator '-=' for Vectors
		 */
		i6eVector2 operator-=(const i6eVector2 & b) {
			setX(_x - b.getX());
			setY(_y - b.getY());
			return *this;
		}

		/**
		 * \brief Operator '!=' for Vector
		 */
		bool operator!=(const i6eVector2 & b) const {
			return !(*this == b);
		}

		/**
		 * \brief returns the normalized Vector
		 */
		i6eVector2 normalize() const {
			if (length() < EPSILON) {
				ISIXE_THROW_API("i6eVector2", "Tried to normalize a (0, 0) vector");
			}
			return i6eVector2(*this / length());
		}

		/**
		 * \brief inserts this vector into an attributeMap using the given prefix
		 */
		void insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) const {
			std::stringstream ss;
			ss << _x << " " << _y;
			map[prefix] = ss.str();
		}

		/**
		 * \brief muliplies two Vectors per component
		 * \param[in] b
		 */
		void mulComponents(const i6eVector2 & b) {
			_x = _x * b.getX();
			_y = _y * b.getY();
		}

		/**
		 * \brief determins whether the vector contains valid data
		 * This has to be set by the user. Operations resulting in components becoming
		 * NaN or Infinity will NOT change this flag
		 */
		bool isValid() const {
			return _valid;
		}
		void setValid(bool b) {
			_valid = b;
		}

		/**
		 * \brief length of the Vector
		 */
		T length() const {
			return T(std::sqrt(_x * _x + _y * _y));
		}

		/**
		 * \brief converts Vector to std::string
		 *
		 * converts the vector to a std::string using the Format "x y z"
		 */
		std::string toString() const {
			std::stringstream s;
			s.precision(100);
			s << _x << " " << _y;
			return s.str();
		}

		/**
		 * \brief serializer for the vector
		 */
		template<class Archive>
		void serialize(Archive & ar, const unsigned int) {
			ar & _x;
			ar & _y;
		}

	private:
		T _x, _y;
		bool _valid;
	};

	template<typename T>
	const double i6eVector2<T>::EPSILON = 1e-15;

	template<typename T>
	const i6eVector2<T> i6eVector2<T>::ZERO = i6eVector2<T>(T(), T());

	template<typename T>
	typename std::enable_if<std::is_integral<T>::value, bool>::type operator==(const i6eVector2<T> & first, const i6eVector2<T> & second) {
		return first.getX() == second.getX() && first.getY() == second.getY();
	}

	template<typename T>
	typename std::enable_if<std::is_floating_point<T>::value, bool>::type operator==(const i6eVector2<T> & first, const i6eVector2<T> & second) {
		return std::fabs(first.getX() - second.getX()) < FLT_EPSILON && std::fabs(first.getY() - second.getY()) < FLT_EPSILON;
	}

} /* namespace math */
} /* namespace i6e */

typedef i6e::math::i6eVector2<double> Vec2;
typedef i6e::math::i6eVector2<float> Vec2f;
typedef i6e::math::i6eVector2<int32_t> Vec2i;
typedef i6e::math::i6eVector2<uint32_t> Vec2ui;

/**
 * \brief stream operator for the i6eVector2
 */
ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const Vec2 & v);
ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const Vec2f & v);
ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const Vec2i & v);
ISIXE_MATH_API std::ostream & operator<<(std::ostream & stream, const Vec2ui & v);

#endif /* __I6ENGINE_MATH_I6EVECTOR2_H__ */

/**
 * @}
 */
