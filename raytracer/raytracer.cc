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

// Ein "Objekt", z.B. eine Kugel oder ein Dreieck, und dem zugehörigen Material der Oberfläche.
// Im Prinzip ein Wrapper-Objekt, das mindestens Material und geometrisches Objekt zusammenfasst.
// Kugel und Dreieck finden Sie in geometry.h/tcc

// verschiedene Materialdefinition, z.B. Mattes Schwarz, Mattes Rot, Reflektierendes Weiss, ...
// im wesentlichen Variablen, die mit Konstruktoraufrufen initialisiert werden.

// Die folgenden Werte zur konkreten Objekten, Lichtquellen und Funktionen, wie Lambertian-Shading
// oder die Suche nach einem Sehstrahl für das dem Augenpunkt am nächsten liegenden Objekte,
// können auch zusammen in eine Datenstruktur für die gesammte zu
// rendernde "Szene" zusammengefasst werden.

// Die Cornelbox aufgebaut aus den Objekten
// Am besten verwendet man hier einen std::vector< ... > von Objekten.

// Punktförmige "Lichtquellen" können einfach als vec3 implementiert werden mit weisser Farbe,
// bei farbigen Lichtquellen müssen die entsprechenden Daten in Objekt zusammengefaßt werden
// Bei mehreren Lichtquellen können diese in einen std::vector gespeichert werden.

// Sie benötigen eine Implementierung von Lambertian-Shading, z.B. als Funktion
// Benötigte Werte können als Parameter übergeben werden, oder wenn diese Funktion eine Objektmethode eines
// Szene-Objekts ist, dann kann auf die Werte teilweise direkt zugegriffen werden.
// Bei mehreren Lichtquellen muss der resultierende diffuse Farbanteil durch die Anzahl Lichtquellen geteilt werden.

// Für einen Sehstrahl aus allen Objekte, dasjenige finden, das dem Augenpunkt am nächsten liegt.
// Am besten einen Zeiger auf das Objekt zurückgeben. Wenn dieser nullptr ist, dann gibt es kein sichtbares Objekt.

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
  world.push_back({{{-100000, 0, 0}, 99990}, red, 0.25f});
  world.push_back({{{100000, 0, 0}, 99990}, green, 0.25f});
  world.push_back({{{-1, -7, -30}, 3}, blue, 0.3f});

  lights.push_back({{-5.0f, 7.0f, -45}, 1.0f});
  lights.push_back({{5.0f, 7.0f, -45}, 0.2f});
  //lights.push_back({{0.0f, 7.0f, -30}, 1.0f});

}

bool hit_anything(ray3 to_light, std::vector<hitable> world)
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

color lambertian(ray3 ray, hitable closest, float closest_t, std::vector<hitable> world, std::vector<light> lights)
{
  if (closest.sphere.radius != -1)
  {
    point3 hitpoint = ray.origin + closest_t * ray.direction;
    vec3 surface_normal = hitpoint - closest.sphere.center;
    surface_normal.normalize();
    float light_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++)
    {
      vec3 to_light_direction = lights[i].pos - hitpoint;
      vec3 to_light_normalized = to_light_direction;
      to_light_normalized.normalize();
      ray3 to_light = {hitpoint + 0.08f * to_light_normalized, 0.92f * to_light_direction};
      if (!hit_anything(to_light, world))
      {
        light_intensity += lights[i].intensity * std::max(0.0f, surface_normal * to_light_normalized);
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

color ray_color(ray3 ray, int depth, std::vector<hitable> world, std::vector<light> lights)
{
  if (depth <= 0)
    return {0.0f, 0.0f, 0.0f};

  hitable closest = {{{0.0f, 0.0f, 0.0f}, -1}, {0.0f, 0.0f, 0.0f}, 0.0f};
  float closest_t = std::numeric_limits<float>::max();
  for (auto obj : world)
  {
    float t = obj.sphere.intersects(ray);
    if (t > 0.0f)
    {
      if (t < closest_t)
      {
        closest = obj;
        closest_t = t;
      }
    }
  }
  return lambertian(ray, closest, closest_t, world, lights);
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
      color pixel_color = ray_color(ray, 1, world, lights);
      file.writeColor(pixel_color);
    }
  }

  return 0;
}