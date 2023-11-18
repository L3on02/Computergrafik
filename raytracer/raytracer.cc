#include "camera.h"
#include "fileout.h"
#include "render.h"

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


  //world.push_back({{{-4, -6.5f, -30}, 4}, blue, 0.3f, 1.52f, true, false});
  world.push_back({{{4, -6.5f, -30}, 4}, green, 0.3f, 1.52f, true, true});

  lights.push_back({{-5.0f, 4.0f, -45}, 1.0f});
  lights.push_back({{5.0f, 7.0f, -45}, 1.0f});
  // lights.push_back({{0.0f, 7.0f, -15}, 1.0f});
}

int main(void)
{
  int image_width = 960;
  int max_depth = 10;

  point3 cam_center = {0.0f, 0.0f, 0.0f};
  float focal_length = 1.0f;
  float vfov = 90.0f;
  float aspect_ratio = 16.0f / 9.0f;
  int image_height = image_width / aspect_ratio;

  camera cam(cam_center, focal_length, vfov, image_width, image_height, aspect_ratio);

  std::vector<hitable> world;
  std::vector<light> lights;
  initialize_world(world, lights);

  fileout file(image_width, image_height);

  image_monitor monitor(image_width, image_height);

  start_render(image_width, image_height, max_depth, world, lights, cam, monitor);

  file.writeImage(monitor.get_image(), image_width, image_height);

  return 0;
}