#include "geometry.h"
#include "geometry.tcc"

template class Intersection_Context<float,3u>;

template class Ray<float, 2u>;
template class Ray<float, 3u>; 

template class AxisAlignedBoundingBox<float, 2u>;
template class AxisAlignedBoundingBox<float, 3u>; 

template class Sphere<float, 2u>;
template class Sphere<float, 3u>; 

template class Triangle<float, 3u>; 

template bool refract<float, 3u>(float refraction_index, Vector<float, 3u> normal, Vector<float, 3u> direction, Vector<float, 3> & transmission);
