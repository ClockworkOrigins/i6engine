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

#ifndef __I6ENGINE_MATH_I6EVECTOR2_H__
#define __I6ENGINE_MATH_I6EVECTOR2_H__

#include <cmath>
#include <cstdint>
#include <map>
#include <string>

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Exceptions.h"

namespace i6engine {
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
		 * \brief Operator '==' for Vector
		 */
		bool operator==(const i6eVector2 & b) const {
			return std::fabs(_x - b.getX()) < EPSILON && std::fabs(_y - b.getY()) < EPSILON;
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
		void insertInMap(const std::string & prefix, std::map<std::string, std::string> & map) {
			std::stringstream ss;
			ss << _x << " " << _y;
			map[prefix] = ss.str();
		}

		/**
		 * \brief muliplies two Vectors per component
		 * \param b
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
		double length() const {
			return std::sqrt(_x * _x + _y * _y);
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
		void serialize(Archive & ar, const unsigned int version) {
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

	template<>
	bool i6eVector2<int32_t>::operator==(const i6eVector2<int32_t> & b) const;

	template<>
	bool i6eVector2<uint32_t>::operator==(const i6eVector2<uint32_t> & b) const;

} /* namespace math */
} /* namespace i6engine */

typedef i6engine::math::i6eVector2<double> Vec2;
typedef i6engine::math::i6eVector2<float> Vec2f;
typedef i6engine::math::i6eVector2<int32_t> Vec2i;
typedef i6engine::math::i6eVector2<uint32_t> Vec2ui;

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
