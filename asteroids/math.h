#ifndef MATH_H
#define MATH_H

#include <initializer_list>
#include <array>
#include <cstddef>
#include <cmath>

// A Vector consisting of N scalar values of type FLOAT_TYPE
template<class FLOAT_TYPE, size_t N>
struct Vector {
  static_assert(N > 0u); // no zero length vectors allowed
  
  // stores the N scalar values of this Vector
  // index 0, 1, 2, ... corresponds to x,y,z,... axis
  std::array<FLOAT_TYPE, N> vector;

  // creates a new Vector with the given scalar values
  // if values is empty, then this->vector is initilized with zeros
  // if less than N values are given, then all remaining values of this->vector
  //   are initialized with the last given value 
  Vector( std::initializer_list<FLOAT_TYPE> values );
  
  // creates a unit vector pointing to the given angle (in radians) in the x/y plane
  // angle = 0 points in the direction of the x-axis
  explicit Vector(FLOAT_TYPE angle);

  // adds addend to this Vector and returns the resulting sum
  Vector & operator+=(const Vector addend);

  // subtracts minuend from this Vector and returns the resulting difference
  Vector & operator-=(const Vector minuend);

  // multiplies the scalar factor to this vector and returns the result
  Vector & operator*=(const FLOAT_TYPE factor);

  // divides this vector by the given factor and returns the result
  Vector & operator/=(const FLOAT_TYPE factor);

  // returns the reference of the i-th scalar component of this vector      
  FLOAT_TYPE & operator[](std::size_t i);

  // returns the i-th scalar component of this Vector
  FLOAT_TYPE operator[](std::size_t i) const;

  // returns the i-th scalar component of this Vector
  // throws an exception if i >= N
  FLOAT_TYPE at(std::size_t i) const;
  
  // normalize this Vector to the length 1  
  void normalize();
  
  // returns the specular reflective "ray" Vector wrt the give normal vector
  // normal must be a normalized vector
  Vector get_reflective(Vector normal) const;
  
  // returns the angle of this Vector between the two given axis in radians
  FLOAT_TYPE angle(size_t axis_1, size_t axis_2) const;

  // returns the cross product of this Vector with the Vector v
  // only three-dimensional case
  Vector<FLOAT_TYPE, 3u> cross_product(const Vector<FLOAT_TYPE, 3u> v) const;
  
  // returns the scalar product of the given scalar and value
  template <class F, size_t K>    
  friend Vector<F, K> operator*(F scalar, Vector<F, K> value);

  // returns the vector sum of the to given vectors
  template <class F, size_t K>    
  friend Vector<F, K> operator+(const Vector<F, K> value, const Vector<F, K> addend);

  // returns the vector difference value - minuend
  template <class F, size_t K>    
  friend Vector<F, K> operator-(const Vector<F, K> value, const Vector<F, K> minuend);


  // returns the (euclidian) length of this Vector
  FLOAT_TYPE length() const;
  

  // returns the square of the this Vector's length
  FLOAT_TYPE square_of_length() const;


  // returns the scalar (inner) product of two Vectors
  template <class F, size_t K>    
  friend F operator*(Vector<F, K> vector1, const Vector<F, K> vector2);
  
};

static const long double PI = std::acos(-1.0L);

// shorter comfortable type names
typedef Vector<float, 2u> Vector2df;
typedef Vector<float, 3u> Vector3df;
typedef Vector<float, 4u> Vector4df;

#endif
