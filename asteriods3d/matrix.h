#ifndef MATRIX_H
#define MATRIX_H

#include "math.h"

// a square matrice implementation
template <class FLOAT, size_t N>
class SquareMatrix
{
  static_assert(N > 0u);
  std::array<Vector<FLOAT, N>, N> matrix; // values are stored in column (a vector) order
public:
  SquareMatrix(std::initializer_list<Vector<FLOAT, N>> values);

  // returns reference to the i-th column vector
  Vector<FLOAT, N> &operator[](std::size_t i);

  // returns i-th column vector
  Vector<FLOAT, N> operator[](std::size_t i) const;

  // returns the value at the given row and column
  FLOAT at(size_t row, size_t column) const;

  // returns the reference value at the given row and column
  FLOAT &at(size_t row, size_t column);

  // returns the producut of this SquareMatrix and the given vector
  Vector<FLOAT, N> operator*(const Vector<FLOAT, N> vector);

  //  returns the product of two square matrices
  template <class F, size_t K>
  friend SquareMatrix<F, K> operator*(const SquareMatrix<F, K> factor1, const SquareMatrix<F, K> factor2);
};

typedef SquareMatrix<float, 2u> sm2;
typedef SquareMatrix<float, 3u> sm3;
typedef SquareMatrix<float, 4u> sm4;

#endif
