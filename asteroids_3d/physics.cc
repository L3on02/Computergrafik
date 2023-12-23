#include "physics.h"
#include "physics.tcc"

template class BoundingVolumeCircle<float, 2>;
template class BoundingVolumeHyperRectangle<float, 2>;
template class Body<float, 2u, BoundingVolumeCircle<float, 2>>;
template class Physics<float, 2u, BoundingVolumeCircle<float, 2>>;
template class Body<float, 2u, BoundingVolumeHyperRectangle<float, 2>>;
template class Physics<float, 2u, BoundingVolumeHyperRectangle<float, 2>>;

