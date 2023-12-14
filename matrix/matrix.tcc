#include <cassert>
#include <random>
#include <iostream>
#include "math.h"
#include "matrix.h"

template <class FLOAT, size_t N>
SquareMatrix<FLOAT, N>::SquareMatrix()
{
    for (size_t i = 0; i < N; ++i)
    {
        matrix[i] = Vector<FLOAT, N>();
    }
}

template <class FLOAT, size_t N>
SquareMatrix<FLOAT, N>::SquareMatrix(std::initializer_list<Vector<FLOAT, N>> values)
{
    auto iterator = values.begin();
    for (size_t i = 0u; i < N; i++)
    {
        if (iterator != values.end())
        {
            matrix[i] = *iterator++;
        }
        else
        {
            matrix[i] = (i > 0 ? matrix[i - 1] : Vector<FLOAT, N>());
        }
    }
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> &SquareMatrix<FLOAT, N>::operator[](std::size_t i)
{
    return matrix[i];
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator[](std::size_t i) const
{
    return matrix[i];
}

template <class FLOAT, size_t N>
inline FLOAT SquareMatrix<FLOAT, N>::at(size_t row, size_t column) const
{
    return matrix[column][row];
}

template <class FLOAT, size_t N>
inline FLOAT &SquareMatrix<FLOAT, N>::at(size_t row, size_t column)
{
    return matrix[column][row];
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator*(const Vector<FLOAT, N> vector)
{
    Vector<FLOAT, N> result;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            result[i] += vector[j] * matrix[j][i];
        }
    }
    return result;
}

template <class F, size_t K>
SquareMatrix<F, K> operator*(const SquareMatrix<F, K>& matrix1, const SquareMatrix<F, K>& matrix2)
{
    SquareMatrix<F, K> result;
    for (size_t i = 0; i < K; i++)
    {
        for (size_t j = 0; j < K; j++)
        {
            for (size_t k = 0; k < K; k++)
            {
                result[j][i] += matrix1[k][i] * matrix2[j][k];
            }
        }
    }
    return result;
}

