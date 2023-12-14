#include <cassert>
#include <random>
#include "math.h"

template <class FLOAT, size_t N>
Vector<FLOAT, N>::Vector()
{
  for (std::size_t i = 0; i < N; ++i)
  {
    vector[i] = 0.0f;
  }
}

template <class FLOAT, size_t N>
Vector<FLOAT, N>::Vector(std::initializer_list<FLOAT> values)
{
  auto iterator = values.begin();
  for (size_t i = 0u; i < N; i++)
  {
    if (iterator != values.end())
    {
      vector[i] = *iterator++;
    }
    else
    {
      vector[i] = (i > 0 ? vector[i - 1] : 0.0);
    }
  }
}

template <class FLOAT, size_t N>
Vector<FLOAT, N>::Vector(FLOAT angle)
{
  *this = {static_cast<FLOAT>(cos(angle)), static_cast<FLOAT>(sin(angle))};
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> &Vector<FLOAT, N>::operator+=(const Vector<FLOAT, N> addend)
{
  for (size_t i = 0u; i < N; i++)
  {
    vector[i] += addend.vector[i];
  }
  return *this;
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> &Vector<FLOAT, N>::operator-=(const Vector<FLOAT, N> minuend)
{
  for (size_t i = 0u; i < N; i++)
  {
    vector[i] -= minuend.vector[i];
  }
  return *this;
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> &Vector<FLOAT, N>::operator*=(const FLOAT factor)
{
  for (size_t i = 0u; i < N; i++)
  {
    vector[i] *= factor;
  }
  return *this;
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> &Vector<FLOAT, N>::operator/=(const FLOAT factor)
{
  for (size_t i = 0u; i < N; i++)
  {
    vector[i] /= factor;
  }
  return *this;
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> operator*(FLOAT scalar, Vector<FLOAT, N> value)
{
  Vector<FLOAT, N> scalar_product = value;

  scalar_product *= scalar;

  return scalar_product;
}
template <class FLOAT, size_t N>
Vector<FLOAT, N> operator*(Vector<FLOAT, N> value, FLOAT scalar)
{
  return scalar * value;
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> operator+(const Vector<FLOAT, N> value, const Vector<FLOAT, N> addend)
{
  Vector<FLOAT, N> sum = value;
  sum += addend;
  return sum;
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> operator-(const Vector<FLOAT, N> value, const Vector<FLOAT, N> minuend)
{
  Vector<FLOAT, N> difference = value;
  difference -= minuend;
  return difference;
}

template <class FLOAT, size_t N>
FLOAT &Vector<FLOAT, N>::operator[](std::size_t i)
{
  return vector[i];
}

template <class FLOAT, size_t N>
FLOAT Vector<FLOAT, N>::operator[](std::size_t i) const
{
  return vector[i];
}

template <class FLOAT, size_t N>
Vector<FLOAT, 3u> Vector<FLOAT, N>::cross_product(const Vector<FLOAT, 3u> v) const
{
  assert(N >= 3u);
  return {this->vector[1] * v.vector[2] - this->vector[2] * v.vector[1],
          this->vector[0] * v.vector[2] - this->vector[2] * v.vector[0],
          this->vector[0] * v.vector[1] - this->vector[1] * v.vector[0]};
}

template <class FLOAT, size_t N>
void Vector<FLOAT, N>::normalize()
{
  *this /= length(); //  +/- INFINITY if length is (near to) zero
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> Vector<FLOAT, N>::get_reflective(Vector<FLOAT, N> normal) const
{
  assert(0.99999 < normal.square_of_length() && normal.square_of_length() < 1.000001);
  return *this - static_cast<FLOAT>(2.0) * (*this * normal) * normal;
}

template <class FLOAT, size_t N>
inline Vector<FLOAT, N> Vector<FLOAT, N>::get_randmo_unit_vector(Vector normal) const
{
  Vector<FLOAT, N> random_vector;
  do
  {
    random_vector = {static_cast<FLOAT>(2.0) * static_cast<FLOAT>(rand()) / static_cast<FLOAT>(RAND_MAX) - static_cast<FLOAT>(1.0),
                     static_cast<FLOAT>(2.0) * static_cast<FLOAT>(rand()) / static_cast<FLOAT>(RAND_MAX) - static_cast<FLOAT>(1.0),
                     static_cast<FLOAT>(2.0) * static_cast<FLOAT>(rand()) / static_cast<FLOAT>(RAND_MAX) - static_cast<FLOAT>(1.0)};
  } while (random_vector * normal > static_cast<FLOAT>(1.0));
  return random_vector;
}

template <class FLOAT, size_t N>
FLOAT Vector<FLOAT, N>::angle(size_t axis_1, size_t axis_2) const
{
  Vector<FLOAT, N> normalized = (1.0f / length()) * *this;
  return atan2(normalized[axis_2], normalized[axis_1]);
}

// Vektorlänge
template <class FLOAT, size_t N>
inline FLOAT Vector<FLOAT, N>::length() const
{
  return sqrt(square_of_length());
}

// Quadratische Vektorlänge
template <class FLOAT, size_t N>
inline FLOAT Vector<FLOAT, N>::square_of_length() const
{
  FLOAT length = 0.0;
  for (size_t i = 0u; i < N; i++)
  {
    length += vector[i] * vector[i];
  }
  return length;
}

// Skalarprodukt
template <class F, size_t K>
F operator*(Vector<F, K> vector1, const Vector<F, K> vector2)
{
  F sc_product = 0.0;
  for (int i = 0; i < K; i++)
  {
    sc_product += vector1[i] * vector2[i];
  }
  return sc_product;
}