#ifndef HITABLE_H
#define HITABLE_H

#include "geometry.h"

#define MATTE_WHITE material{{0.8f, 0.8f, 0.8f}, 0.25f}
#define MATTE_RED material{{0.8f, 0.3f, 0.3f}, 0.25f}
#define MATTE_GREEN material{{0.3f, 0.8f, 0.3f}, 0.25f}
#define MATTE_BLUE material{{0.3f, 0.3f, 0.8f}, 0.25f}
#define MATTE_BLACK material{{0.2f, 0.2f, 0.2f}, 0.25f}

#define MIRROR material{{0.0f, 0.0f, 0.0f}, 0.25f, 1.0f, 0.9f, false}
#define GLASS material{{1.0f, 1.0f, 1.0f}, 0.25f, 1.52f, 0.9f, true}

struct material {
    color col = {0.0f, 0.0f, 0.0f};
    float const_light = 0.3f;
    float density = 1.0f;
    float reflectivity = 0.0f;
    bool is_transmissive = false;
};

struct hitable
{
    sphere3 sphere = {{0.0f, 0.0f, 0.0f}, -1.0f};
    material mat = MATTE_BLACK;
};

struct light
{
    point3 pos;
    float intensity;
};

#endif