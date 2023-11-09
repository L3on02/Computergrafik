#ifndef HITABLE_H
#define HITABLE_H

#include "geometry.h"

struct hitable {
    sphere3 sphere = {{0.0f, 0.0f, 0.0f}, 0.0f};
    color col = {0.0f, 0.0f, 0.0f};
};

#endif