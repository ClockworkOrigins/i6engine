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

#ifndef __I6ENGINE_MATH_I6EMATRIX_H__
#define __I6ENGINE_MATH_I6EMATRIX_H__

#include <vector>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eVector.h"

namespace boost {
namespace serialization {
	class access;
} /* serialization */
} /* boost */

namespace i6e {
namespace math {

	/**
	 * \class i6eMatrix
	 * \brief Implements m x n matrix
	 */
	template<typename T>
	class i6eMatrix {
	public:
		/**
		 * \brief default constructor, creates an emtpy matrix
		 */
		i6eMatrix() : _width(0), _height(0), _data() {
		}

		/**
		 * \brief constructor taking dimension of the matrix setting all values to zero
		 */
		i6eMatrix(uint32_t m, uint32_t n) : _width(n), _height(m), _data(m, std::vector<T>(n, T())) {
		}

		/**
		 * \brief copy constructor
		 */
		i6eMatrix(const i6eMatrix & other) : _width(other.getWidth()), _height(other.getHeight()), _data(other._data) {
		}

		/**
		 * \brief constructor taking an i6eVector
		 */
		explicit i6eMatrix(const i6eVector & vector) : _width(1), _height(3), _data(3, std::vector<T>(1, 0.0)) {
			_data[0][0] = vector.getX();
			_data[1][0] = vector.getY();
			_data[2][0] = vector.getZ();
		}

		/**
		 * \brief multiplication of matrix with a scalar
		 */
		i6eMatrix operator*(const T & value) {
			i6eMatrix result(*this);

			result *= value;

			return result;
		}

		/**
		 * \brief multiplication of matrix with a scalar
		 */
		i6eMatrix & operator*=(const T & value) {
			i6eMatrix result(*this);

			for (unsigned int i = 0; i < _height; ++i) {
				for (unsigned int j = 0; j < _width; ++j) {
					_data[i][j] *= value;
				}
			}

			return *this;
		}

		/**
		 * \brief division of matrix with scalar
		 */
		i6eMatrix operator/(const T & value) {
			i6eMatrix result(*this);

			result /= value;

			return result;
		}

		/**
		 * \brief division of matrix with a scalar
		 */
		i6eMatrix & operator/=(const T & value) {
			i6eMatrix result(*this);

			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					_data[i][j] /= value;
				}
			}

			return *this;
		}

		/**
		 * \brief multiplication with matrix
		 */
		i6eMatrix operator*(const i6eMatrix & other) {
			i6eMatrix result(*this);

			result *= other;

			return result;
		}

		/**
		 * \brief multiplication with matrix
		 */
		i6eMatrix operator*=(const i6eMatrix & other) {
			if (_width != other.getHeight()) {
				ISIXE_THROW_API("i6eMatrix", "wrong dimension for matrix-matrix-multiplication");
			}

			i6eMatrix result(_height, other.getWidth());

			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < other.getWidth(); ++j) {
					for (uint32_t k = 0; k < _width; ++k) {
						result(i, j, result(i, j) + _data[i][k] * other(k, j));
					}
				}
			}

			*this = result;

			return *this;
		}

		/**
		 * \brief operator for addition with matrix
		 */
		i6eMatrix operator+(const i6eMatrix & other) {
			i6eMatrix result(*this);

			result += other;

			return result;
		}

		/**
		 * \brief operator for addition with matrix
		 */
		i6eMatrix & operator+=(const i6eMatrix & other) {
			if (_height != other.getHeight() || _width != other.getWidth()) {
				ISIXE_THROW_API("i6eMatrix", "wrong dimension for matrix-matrix-addition");
			}

			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					_data[i][j] += other(i, j);
				}
			}

			return *this;
		}

		/**
		 * \brief operator for substraction of matrix
		 */
		i6eMatrix operator-(const i6eMatrix & other) {
			i6eMatrix result(*this);

			result -= other;

			return result;
		}

		/**
		 * \brief operator for substraction of matrix
		 */
		i6eMatrix & operator-=(const i6eMatrix & other) {
			if (_height != other.getHeight() || _width != other.getWidth()) {
				ISIXE_THROW_API("i6eMatrix", "wrong dimension for matrix-matrix-subtraction");
			}

			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					_data[i][j] -= other(i, j);
				}
			}

			return *this;
		}

		/**
		 * \brief flips values of the matrix
		 */
		i6eMatrix operator-() {
			i6eMatrix result(*this);

			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					result(i, j, -_data[i][j]);
				}
			}

			return result;
		}

		/**
		 * \brief sets the given value to the given position
		 */
		void operator()(uint32_t m, uint32_t n, const T & value) {
			setEntry(m, n, value);
		}

		/**
		 * \brief returs the value at the given position
		 */
		T operator()(uint32_t m, uint32_t n) const {
			return getEntry(m, n);
		}

		/**
		 * \brief returns true, if both matrixes are equal
		 */
		bool operator==(const i6eMatrix & other) {
			if (_width != other.getWidth() || _height != other.getHeight()) {
				return false;
			}

			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					if (std::fabs(_data[i][j] - other._data[i][j]) > 0.0000001f) {
						return false;
					}
				}
			}

			return true;
		}

		/**
		 * \brief return true, if both matrixes aren't equal
		 */
		bool operator!=(const i6eMatrix & other) const {
			return !(*this == other);
		}

		/**
		 * \brief sets the given value to the given position
		 */
		void setEntry(uint32_t m, uint32_t n, T value) {
			if (m >= _height || n >= _width) {
				ISIXE_THROW_API("i6eMatrix", "field (" << m << ", " << n << ") not part of the matrix");
			}

			_data[m][n] = value;
		}

		/**
		 * \brief sets all values of the matrix to zero
		 */
		void setZero() {
			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					_data[i][j] = T();
				}
			}
		}

		/**
		 * \brief sets matrix to be an identity matrix
		 */
		void setIdentity() {
			for (uint32_t i = 0; i < _height; ++i) {
				for (uint32_t j = 0; j < _width; ++j) {
					if (i == j) {
						_data[i][j] = 1;
					} else {
						_data[i][j] = 0;
					}
				}
			}
		}

		/**
		 * \brief returns width of the matrix
		 */
		inline uint32_t getWidth() const {
			return _width;
		}

		/**
		 * \brief returns height of the matrix
		 */
		inline uint32_t getHeight() const {
			return _height;
		}

		/**
		 * \brief returns entry at given position
		 */
		inline T getEntry(uint32_t m, uint32_t n) const {
			if (m >= _height || n >= _width) {
				ISIXE_THROW_API("i6eMatrix", "field (" << m << ", " << n << ") not part of the matrix");
			}

			return _data[m][n];
		}

		/**
		 * \brief returns determinant of the matrix
		 */
		double calculateDeterminant() const {
			double det = 1.0;

			if (_height != _width) {
				ISIXE_THROW_API("i6eMatrix", "only nxn matrices have a determinant");
			}

			if (getHeight() == 2) {
				return _data[0][0] * _data[1][1] - _data[1][0] * _data[0][1];
			}

			i6eMatrix l(getHeight(), getWidth());
			i6eMatrix u(getHeight(), getWidth());

			decomposeLU(l, u);

			for (uint32_t i = 0; i < getHeight(); ++i) {
				det *= u(i, i);
			}

			return det;
		}

		/**
		 * \brief calculates a given amount of multiplications for the given matrix
		 */
		static i6eMatrix pow(const i6eMatrix & other, uint32_t amount) {
			i6eMatrix result(other);

			for (uint32_t i = 1; i < amount; ++i) {
				result *= other;
			}

			return result;
		}

		/**
		 * \brief calculates the transposed matrix
		 */
		static i6eMatrix transpose(const i6eMatrix & other) {
			i6eMatrix result(other.getWidth(), other.getHeight());

			for (uint32_t i = 0; i < other.getHeight(); ++i) {
				for (uint32_t j = 0; j < other.getWidth(); ++j) {
					result(j, i, other(i, j));
				}
			}

			return result;
		}

		/**
		 * \brief calculates the inverted of the given matrix
		 */
		static i6eMatrix invert(const i6eMatrix & other) {
			if (other.getHeight() != other.getWidth()) {
				ISIXE_THROW_API("i6eMatrix", "only nxn matrices have an inverse");
			}

			T det = T(other.calculateDeterminant());

			assert(std::fabs(det) > 1e-15);

			return adjoint(other) * (1.0f / det);
		}

		/**
		 * \brief calculates the adjoint of the given matrix
		 */
		static i6eMatrix adjoint(const i6eMatrix & other) {
			if (other.getHeight() != other.getWidth()) {
				ISIXE_THROW_API("i6eMatrix", "only nxn matrices have an adjoint");
			}

			i6eMatrix result(other.getHeight(), other.getWidth());

			for (uint32_t i = 0; i < other.getHeight(); ++i) {
				for (uint32_t j = 0; j < other.getWidth(); ++j) {
					i6eMatrix m(other.getHeight() - 1, other.getWidth() - 1);

					uint32_t rowC = 0;
					uint32_t columnC = 0;

					for (uint32_t x = 0; x < other.getHeight(); ++x) {
						if (x == i) {
							continue;
						}
						for (uint32_t y = 0; y < other.getWidth(); ++y) {
							if (y == j) {
								continue;
							}

							m(rowC, columnC, other(x, y));

							columnC++;
						}
						rowC++;
						columnC = 0;
					}

					T k = T(std::pow(-1.0f, i + j) * m.calculateDeterminant());
					result(i, j, k);
				}
			}

			return transpose(result);
		}

		/**
		 * \brief solves the linear system mx = b using LU-decomposition, where m is the input square matrix and x is the result
		 */
		void solveSystem(const i6eMatrix & b, i6eMatrix & x) {
			if (getHeight() != getWidth()) {
				ISIXE_THROW_API("i6eMatrix", "only nxn matrices can be solved using LU decomposition");
			}

			i6eMatrix l(*this);
			i6eMatrix u(*this);

			decomposeLU(l, u);

			i6eMatrix y(getHeight(), 1);

			forwardSubstitution(l, b, y);
			backwardSubstitution(u, y, x);
		}

	private:
		friend class boost::serialization::access;

		uint32_t _width;
		uint32_t _height;
		std::vector<std::vector<T>> _data;

		/**
		 * \brief calculates L and U of LU-decomposition
		 */
		void decomposeLU(i6eMatrix & l, i6eMatrix & u) const {
			assert(getWidth() == getHeight());

			l.setZero();
			u = *this;

			for (uint32_t i = 0; i < l.getWidth(); ++i) {
				l(i, i, 1.0f);
			}

			for (uint32_t i = 0; i < getHeight(); ++i) {
				for (uint32_t j = i + 1; j < getHeight(); ++j) {
					l(j, i, u(j, i) / u(i, i));

					for (uint32_t k = i; k < getWidth(); ++k) {
						u(j, k, u(j, k) - l(j, i) * u(i, k));
					}
				}
			}
		}

		/**
		 * \brief forward substitution solves ly = b where y is the result
		 */
		void forwardSubstitution(const i6eMatrix & l, const i6eMatrix & b, i6eMatrix & y) {
			assert(l.getWidth() == l.getHeight());
			assert(l.getHeight() == b.getHeight() && b.getWidth() == 1);

			y = i6eMatrix(l.getHeight(), 1);

			for (uint32_t i = 0; i < l.getWidth(); ++i) {
				T tmp = b(i, 0);

				for (uint32_t j = 0; j < i; ++j) {
					tmp -= y(j, 0) * l(i, j);
				}

				y(i, 0, tmp);
			}
		}

		/**
		 * \brief backward substitution solves ux = y where x is the result
		 */
		void backwardSubstitution(const i6eMatrix & u, const i6eMatrix & y, i6eMatrix & x) {
			assert(u.getWidth() == u.getHeight());
			assert(y.getHeight() == u.getHeight() && y.getWidth() == 1);

			x = i6eMatrix(u.getHeight(), 1);

			for (uint32_t i = u.getHeight(); i > 0; --i) {
				T tmp = y(i - 1, 0);

				for (uint32_t j = u.getHeight() - 1; j > i - 1; --j) {
					tmp -= x(j, 0) * u(i - 1, j);
				}

				x(i - 1, 0, tmp / u(i - 1, i - 1));

				if (i == 0) {
					break;
				}
			}
		}

		/**
		 * \brief archives the matrix
		 */
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & _width;
			ar & _height;
			ar & _data;
		}
	};

	template<>
	i6eMatrix<float> i6eMatrix<float>::operator/(const float & value) {
		i6eMatrix result(*this);

		result *= 1.0f / value;

		return result;
	}

	template<>
	i6eMatrix<float> & i6eMatrix<float>::operator/=(const float & value) {
		*this *= 1.0f / value;

		return *this;
	}

} /* namespace math */
} /* namespace i6e */

#endif /* __I6ENGINE_MATH_I6EMATRIX_H__ */

/**
 * @}
 */
