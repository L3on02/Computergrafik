#ifndef HITABLE_H
#define HITABLE_H

#include "geometry.h"

struct hitable
{
    sphere3 sphere;
    color col;
    float const_light;
    float density = 1.0f;
    bool is_reflective = false;
    bool is_transmissive = false;
};

struct light
{
    point3 pos;
    //color col;
    float intensity;
};

#endif