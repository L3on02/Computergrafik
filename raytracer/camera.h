#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"

class camera
{
public:
    camera(point3 _center, float _focal_length, float vfov, int image_width, int image_height, float aspect_ratio) : center(_center), focal_length(_focal_length)
    {
        float theta = vfov * M_PI / 180; // convert to radians
        float h = tan(theta / 2);
        float viewport_height = h * focal_length;
        float viewport_width = viewport_height * aspect_ratio;

        vec3 viewport_u = {viewport_width, 0.0f, 0.0f};
        vec3 viewport_v = {0.0f, -viewport_height, 0.0f};

        vec3 focal_point = {0.0f, 0.0f, focal_length};
        w = (center - focal_point);
        vec3 viewport_upper_left = center + w - (viewport_width / 2) * u + (viewport_height / 2) * v;

        pixel_delta_u = (1.0f / image_width) * viewport_u;
        pixel_delta_v = (1.0f / image_height) * viewport_v;
        pixel_upper_left = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
    }

    ray3 get_ray(int i, int j) const
    {
        ray3 ray = {center, pixel_upper_left + (float)i * pixel_delta_u + (float)j * pixel_delta_v - center};
        return ray;
    }

private:
    point3 center;
    float focal_length;
    vec3 u = {1.0f, 0.0f, 0.0f}; // x
    vec3 v = {0.0f, 1.0f, 0.0f}; // y
    vec3 w;                      // z
    vec3 pixel_delta_v;
    vec3 pixel_delta_u;
    vec3 pixel_upper_left;
};

#endif