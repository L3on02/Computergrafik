#ifndef HITABLE_H
#define HITABLE_H

#include "geometry.h"

struct hitable
{
    sphere3 sphere;
    color col;
    float const_light;
    // material mat_ptr;
};

struct light
{
    point3 pos;
    //color col;
    float intensity;
};

#endif