#include "matrix.h"
#include "matrix.tcc"

template class SquareMatrix<float, 2u>;
template class SquareMatrix<float, 3u>;
template class SquareMatrix<float, 4u>;

template SquareMatrix<float, 2> operator*(const SquareMatrix<float, 2> &factor1, const SquareMatrix<float, 2> &factor2);
template SquareMatrix<float, 3> operator*(const SquareMatrix<float, 3> &factor1, const SquareMatrix<float, 3> &factor2);
template SquareMatrix<float, 4> operator*(const SquareMatrix<float, 4> &factor1, const SquareMatrix<float, 4> &factor2);
