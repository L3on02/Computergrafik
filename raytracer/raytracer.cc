#include "geometry.h"
#include "camera.h"
#include "fileout.h"
#include "hitable.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <thread>

// Die folgenden Kommentare beschreiben Datenstrukturen und Funktionen
// Die Datenstrukturen und Funktionen die weiter hinten im Text beschrieben sind,
// hängen höchstens von den vorhergehenden Datenstrukturen ab, aber nicht umgekehrt.

// Das "Material" der Objektoberfläche mit ambienten, diffusem und reflektiven Farbanteil.

// verschiedene Materialdefinition, z.B. Mattes Schwarz, Mattes Rot, Reflektierendes Weiss, ...
// im wesentlichen Variablen, die mit Konstruktoraufrufen initialisiert werden.

// Die rekursive raytracing-Methode. Am besten ab einer bestimmten Rekursionstiefe (z.B. als Parameter übergeben) abbrechen.

void initialize_world(std::vector<hitable> &world, std::vector<light> &lights)
{
  color red = {0.8f, 0.3f, 0.3f};
  color green = {0.3f, 0.8f, 0.3f};
  color white = {0.8f, 0.8f, 0.8f};
  color blue = {0.3f, 0.3f, 0.8f};

  world.push_back({{{0, -100000, 0}, 99990}, white, 0.25});
  world.push_back({{{0, 100000, 0}, 99990}, white, 0.25f});
  world.push_back({{{0, 0, -100000}, 99950}, white, 0.25f});
  world.push_back({{{0, 0, 100000}, 99999}, white, 0.25f});
  world.push_back({{{-100000, 0, 0}, 99990}, red, 0.25f});
  world.push_back({{{100000, 0, 0}, 99990}, green, 0.25f});
  world.push_back({{{-4, -7, -30}, 3}, blue, 0.3f, 1.52f, true, false});

  lights.push_back({{-5.0f, 7.0f, -45}, 1.0f});
  lights.push_back({{5.0f, 7.0f, -45}, 0.2f});
  // lights.push_back({{0.0f, 7.0f, -30}, 1.0f});
}

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

int main(void)
{
  point3 cam_center = {0.0f, 0.0f, 0.0f};
  float focal_length = 1.0f;
  float vfov = 90.0f;
  int image_width = 600;
  float aspect_ratio = 1.0f; // 16.0f / 9.0f;
  int image_height = image_width / aspect_ratio;

  camera cam(cam_center, focal_length, vfov, image_width, image_height, aspect_ratio);

  std::vector<hitable> world;
  std::vector<light> lights;
  initialize_world(world, lights);

  fileout file(image_width, image_height);

  for (int i = 0; i < image_height; i++)
  {
    for (int j = 0; j < image_width; j++)
    {
      ray3 ray = cam.get_ray(i, j);
      color pixel_color = ray_color(ray, 2, world, lights);
      file.writeColor(pixel_color);
    }
  }

  return 0;
}