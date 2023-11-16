#ifndef RENDER_H
#define RENDER_H

#include "geometry.h"
#include "hitable.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <thread>

bool hit_anything(ray3 to_light, std::vector<hitable> &world)
{
  for (auto obj : world)
  {
    float t = obj.sphere.intersects(to_light);
    if (0 < t && t < 1)
    {
      return true;
    }
  }
  return false;
}

color lambertian(hitable closest, Intersection_Context<float, 3> context, std::vector<hitable> &world, std::vector<light> &lights)
{
  if (closest.sphere.radius != -1)
  {
    float light_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++)
    {
      vec3 to_light_direction = lights[i].pos - context.intersection;
      vec3 to_light_normalized = to_light_direction;
      to_light_normalized.normalize();
      ray3 to_light = {context.intersection + 0.08f * to_light_normalized, 0.92f * to_light_direction};
      if (!hit_anything(to_light, world))
      {
        light_intensity += lights[i].intensity * std::max(0.0f, context.normal * to_light_normalized);
      }
    }
    light_intensity /= lights.size();
    return (closest.const_light + light_intensity) * closest.col;
  }
  else
  {
    return {0.0f, 0.0f, 0.0f};
  }
}


float schlick_approximation(vec3 a, vec3 b, hitable A, hitable B)
{
  float r0 = (A.density - B.density) / (A.density + B.density);
  r0 *= r0;
  float cos_theta = a * b;
  if (A.density > B.density)
  {
    float n = A.density / B.density;
    float sin_theta_t = n * n * (1.0f - cos_theta * cos_theta);
    if (sin_theta_t > 1.0f)
    {
      return 1.0f;
    }
    cos_theta = sqrt(1.0f - sin_theta_t);
  }
  float x = 1.0f - cos_theta;
  return r0 + (1.0f - r0) * x * x * x * x * x;
}

ray3 refract(ray3 in, hitable object, Intersection_Context<float, 3> context)
{
  ray3 out;
  out.origin = context.intersection;
  float n = (context.normal * in.direction) > 0 ? object.density / 1.0f : 1.0f / object.density;
  out.direction = n * (in.direction - (context.normal * in.direction) * context.normal) - (float)sqrt(1.0f - n * n * (1.0f - (context.normal * in.direction) * (context.normal * in.direction))) * context.normal;
  return out;
}

color ray_color(ray3 ray, int depth, std::vector<hitable> &world, std::vector<light> &lights)
{
  if (depth <= 0)
    return {0.0f, 0.0f, 0.0f};

  hitable closest = {{{0.0f, 0.0f, 0.0f}, -1}, {0.0f, 0.0f, 0.0f}, 0.0f};
  float closest_t = std::numeric_limits<float>::max();
  Intersection_Context<float, 3> context;
  for (auto obj : world)
  {
    Intersection_Context<float, 3> temp_context;
    if (obj.sphere.intersects(ray, temp_context))
    {
      if (temp_context.t < closest_t)
      {
        closest = obj;
        closest_t = temp_context.t;
        context = temp_context;
      }
    }
  }

  color col = {0, 0, 0};
  if (closest.is_reflective)
  {
    col += ray_color({context.intersection + 0.08f * context.normal, 0.92f * ray.direction.get_reflective(context.normal)}, depth - 1, world, lights);
  }
  
  if (closest.is_transmissive)
  {
    float r = schlick_approximation(ray.direction, context.normal, closest, {{{0, 0, 0}, 0}, {0, 0, 0}, 0, 1});
    col *= (1 - r);
    col += r * ray_color(refract(ray, closest, context), depth - 1, world, lights);
  }
  
  if (!(closest.is_reflective || closest.is_transmissive))
    col += lambertian(closest, context, world, lights);
  
  return col;
}

/* void start_render(int image_height, int image_width, int max_depth, std::vector<hitable> &world, std::vector<light> &lights, camera &cam ,image_memory &image) {
  for (int i = 0; i < image_height; i++)
  {
    for (int j = 0; j < image_width; j++)
    {
      ray3 ray = cam.get_ray(i, j);
      color pixel_color = ray_color(ray, max_depth, world, lights);
      
    }
  }
} */

void render() {

}

#endif