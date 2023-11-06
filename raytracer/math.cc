#include "math.h"
#include "math.tcc"

// contains template instantiations for the 2-, 3- and 4-dimensional cases
//   to create pre-compiled object files

// instantiations of each template class/struct
template class Vector<float, 2u>;
template class Vector<float, 3u>; 
template class Vector<float, 4u>;


// instantiations of each template function
template Vector<float, 2u> operator*(float scalar, Vector<float, 2u> value);
template Vector<float, 2u> operator+(Vector<float, 2u> value, const Vector<float, 2u> addend);
template Vector<float, 2u> operator-(Vector<float, 2u> value, const Vector<float, 2u> addend);

template float operator*(Vector<float, 2u> value, const Vector<float, 2u> addend);

template Vector<float, 3u> operator*(float scalar, Vector<float, 3u> value);
template Vector<float, 3u> operator+(Vector<float, 3u> value, const Vector<float, 3u> addend);
template Vector<float, 3u> operator-(Vector<float, 3u> value, const Vector<float, 3u> addend);

template float operator*(Vector<float, 3u> value, const Vector<float, 3u> addend);

template Vector<float, 4u> operator*(float scalar, Vector<float, 4u> value);
template Vector<float, 4u> operator+(Vector<float, 4u> value, const Vector<float, 4u> addend);
template Vector<float, 4u> operator-(Vector<float, 4u> value, const Vector<float, 4u> addend);

template float operator*(Vector<float, 4u> value, const Vector<float, 4u> addend);


