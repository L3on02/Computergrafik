#ifndef RENDER_H
#define RENDER_H

#include "hitable.h"
#include "parallel.h"

#include <algorithm>
#include <limits>

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
    return (closest.mat.const_light + light_intensity) * closest.mat.col;
  }
  else
  {
    return {0.0f, 0.0f, 0.0f};
  }
}

float schlick_approximation(vec3 inbound, vec3 normal, hitable obj) {
  float r0 = (normal * inbound) > 0 ?(1.0f - obj.mat.density) / (1.0f + obj.mat.density) : (obj.mat.density - 1.0f) / (obj.mat.density + 1.0f);
  r0 *= r0;
  float cos_x = -1.0f * (normal * inbound);
  if (obj.mat.density > 1.0f) {
    float n = obj.mat.density;
    float sin_t2 = n * n * (1.0f - cos_x * cos_x);
    if (sin_t2 > 1.0f) {
      return 1.0f;
    }
    cos_x = (float)sqrt(1.0f - sin_t2);
  }
  float x = 1.0f - cos_x;
  return r0 + (1.0f - r0) * x * x * x * x * x;
}

bool refract(ray3 in, ray3 &out, hitable object, Intersection_Context<float, 3> context)
{
  float n = (context.normal * in.direction) > 0 ? object.mat.density / 1.0f : 1.0f / object.mat.density;
  float root_term = 1.0f - n * n * (1.0f - (context.normal * in.direction) * (context.normal * in.direction));
  if (root_term < 0.0f) { // total internal reflection
    return false;
  }
  vec3 dir = n * (in.direction - (context.normal * in.direction) * context.normal) - (float)sqrt(root_term) * context.normal;
  out.origin = context.intersection;
  out.direction = dir;
  out.origin += 0.08f * out.direction;
  return true;
}

color ray_color(ray3 ray, int depth, std::vector<hitable> &world, std::vector<light> &lights)
{
  if (depth <= 0)
    return {0.0f, 0.0f, 0.0f};
  
  hitable closest;
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
  if (closest.mat.is_reflective)
  {
    col += ray_color({context.intersection + 0.08f * context.normal, 0.92f * ray.direction.get_reflective(context.normal)}, depth - 1, world, lights);
  }

  if (closest.mat.is_transmissive)
  {
    float r = schlick_approximation(ray.direction, context.normal, closest);
    ray3 refracted;
    if(refract(ray, refracted, closest, context)) {
      col *= r;
      col += (1 - r) * ray_color(refracted, depth - 1, world, lights);
    }
  }

  if (!(closest.mat.is_reflective || closest.mat.is_transmissive))
    col += lambertian(closest, context, world, lights);

  return col;
}

void render(int image_width, int image_height, int max_depth, std::vector<hitable> &world, std::vector<light> &lights, camera &cam, image_monitor &monitor)
{
  int i;
  while ((i = monitor.get_render_line()) < image_height)
  {
    for (int j = 0; j < image_width; j++)
    {
      ray3 ray = cam.get_ray(i, j);
      color pixel_color = ray_color(ray, max_depth, world, lights);
      monitor.write_pixel(i, j, pixel_color);
    }
  }
}

void start_render(int image_width, int image_height, int max_depth, std::vector<hitable> &world, std::vector<light> &lights, camera &cam, image_monitor &monitor)
{
  std::clog << "Starting render...\n";
  int processor_count = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;

  for (int i = 0; i < processor_count; i++)
  {
    threads.push_back(std::thread(render, image_width, image_height, max_depth, std::ref(world), std::ref(lights), std::ref(cam), std::ref(monitor)));
  }
  //wait for all threads to finish
  for (auto &t : threads)
  {
    t.join();
  }
  std::clog << "\nDone\n";
}

#endif