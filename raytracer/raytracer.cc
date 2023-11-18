#include "camera.h"
#include "fileout.h"
#include "render.h"

void initialize_world(std::vector<hitable> &world, std::vector<light> &lights)
{
  world.push_back({{{0, -100000, 0}, 99990}, MATTE_WHITE});
  world.push_back({{{0, 100000, 0}, 99990}, MATTE_WHITE});
  world.push_back({{{0, 0, -100000}, 99950}, MATTE_WHITE});
  world.push_back({{{0, 0, 100000}, 99999}, MATTE_WHITE});
  world.push_back({{{-100000, 0, 0}, 99990}, MATTE_RED});
  world.push_back({{{100000, 0, 0}, 99990}, MATTE_GREEN});


  world.push_back({{{-4, -6.5f, -30}, 4}, MIRROR});
  world.push_back({{{4, -6.5f, -30}, 4}, GLASS});

  lights.push_back({{-5.0f, 4.0f, -45}, 1.0f});
  lights.push_back({{5.0f, 7.0f, -45}, 1.0f});
  lights.push_back({{0.0f, 0.0f, -15}, 1.0f});
}

int main(void)
{
  int image_width = 960;
  int max_depth = 50;

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