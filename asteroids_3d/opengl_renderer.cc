#include "opengl_renderer.h"
#include <cassert>
#include <span>
#include <utility>
#include <fstream>
#include "wavefront.h"

// geometric data as in original game and game coordinates
std::vector<vec2> spaceship = {
    vec2{-6.0f, 3.0f},
    vec2{-6.0f, -3.0f},
    vec2{-10.0f, -6.0f},
    vec2{14.0f, 0.0f},
    vec2{-10.0f, 6.0f},
    vec2{-6.0f, 3.0f}};

std::vector<vec2> flame = {
    vec2{-6, 3},
    vec2{-12, 0},
    vec2{-6, -3}};

std::vector<vec2> torpedo_points = {
    vec2{0, 0},
    vec2{0, 1}};

std::vector<vec2> saucer_points = {
    vec2{-16, -6},
    vec2{16, -6},
    vec2{40, 6},
    vec2{-40, 6},
    vec2{-16, 18},
    vec2{16, 18},
    vec2{40, 6},
    vec2{16, -6},
    vec2{8, -18},
    vec2{-8, -18},
    vec2{-16, -6},
    vec2{-40, 6}};

std::vector<vec2> asteroid_1 = {
    vec2{0, -12},
    vec2{16, -24},
    vec2{32, -12},
    vec2{24, 0},
    vec2{32, 12},
    vec2{8, 24},
    vec2{-16, 24},
    vec2{-32, 12},
    vec2{-32, -12},
    vec2{-16, -24},
    vec2{0, -12}};

std::vector<vec2> asteroid_2 = {
    vec2{6, -6},
    vec2{32, -12},
    vec2{16, -24},
    vec2{0, -16},
    vec2{-16, -24},
    vec2{-24, -12},
    vec2{-16, -0},
    vec2{-32, 12},
    vec2{-16, 24},
    vec2{-8, 16},
    vec2{16, 24},
    vec2{32, 6},
    vec2{16, -6},
};

std::vector<vec2> asteroid_3 = {
    vec2{-16, 0},
    vec2{-32, 6},
    vec2{-16, 24},
    vec2{0, 6},
    vec2{0, 24},
    vec2{16, 24},
    vec2{32, 6},
    vec2{32, 6},
    vec2{16, -24},
    vec2{-8, -24},
    vec2{-32, -6}};

std::vector<vec2> asteroid_4 = {
    vec2{8, 0},
    vec2{32, -6},
    vec2{32, -12},
    vec2{8, -24},
    vec2{-16, -24},
    vec2{-8, -12},
    vec2{-32, -12},
    vec2{-32, 12},
    vec2{-16, 24},
    vec2{8, 16},
    vec2{16, 24},
    vec2{32, 12},
    vec2{8, 0}};

std::vector<vec2> spaceship_debris = {
    vec2{-2, -1},
    vec2{-10, 7},
    vec2{3, 1},
    vec2{7, 8},
    vec2{0, 3},
    vec2{6, 1},
    vec2{3, -1},
    vec2{-5, -7},
    vec2{0, -4},
    vec2{-6, -6},
    vec2{-2, 2},
    vec2{2, 5}};

std::vector<vec2> spaceship_debris_direction = {
    vec2{-40, -23},
    vec2{50, 15},
    vec2{0, 45},
    vec2{60, -15},
    vec2{10, -52},
    vec2{-40, 30}};

std::vector<vec2> debris_points = {
    vec2{-32, 32},
    vec2{-32, -16},
    vec2{-16, 0},
    vec2{-16, -32},
    vec2{-8, 24},
    vec2{8, -24},
    vec2{24, 32},
    vec2{24, -24},
    vec2{24, -32},
    vec2{32, -8}};

std::vector<vec2> digit_0 = {{0, -8}, {4, -8}, {4, 0}, {0, 0}, {0, -8}};
std::vector<vec2> digit_1 = {{4, 0}, {4, -8}};
std::vector<vec2> digit_2 = {{0, -8}, {4, -8}, {4, -4}, {0, -4}, {0, 0}, {4, 0}};
std::vector<vec2> digit_3 = {{0, 0}, {4, 0}, {4, -4}, {0, -4}, {4, -4}, {4, -8}, {0, -8}};
std::vector<vec2> digit_4 = {{4, 0}, {4, -8}, {4, -4}, {0, -4}, {0, -8}};
std::vector<vec2> digit_5 = {{0, 0}, {4, 0}, {4, -4}, {0, -4}, {0, -8}, {4, -8}};
std::vector<vec2> digit_6 = {{0, -8}, {0, 0}, {4, 0}, {4, -4}, {0, -4}};
std::vector<vec2> digit_7 = {{0, -8}, {4, -8}, {4, 0}};
std::vector<vec2> digit_8 = {{0, -8}, {4, -8}, {4, 0}, {0, 0}, {0, -8}, {0, -4}, {4, -4}};
std::vector<vec2> digit_9 = {{4, 0}, {4, -8}, {0, -8}, {0, -4}, {4, -4}};

std::vector<std::vector<vec2> *> vertice_data = {
    &spaceship, &flame,
    &torpedo_points, &saucer_points,
    &asteroid_1, &asteroid_2, &asteroid_3, &asteroid_4,
    &spaceship_debris, &spaceship_debris_direction,
    &debris_points,
    &digit_0, &digit_1, &digit_2, &digit_3, &digit_4, &digit_5, &digit_6, &digit_7, &digit_8, &digit_9};

// class OpenGLView
// Constants for attribute sizes and offsets
const GLint vertexSize = 3;
const GLint colorSize = 3;
const GLint normalSize = 3;
const GLsizei stride = 9 * sizeof(float);

OpenGLView::OpenGLView(GLuint vbo, unsigned int shaderProgram, size_t vertices_size, GLuint mode, bool is_3d)
    : shaderProgram(shaderProgram), vertices_size(vertices_size), mode(mode)
{
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  if (is_3d)
  {

    glVertexAttribPointer(0,
                          vertexSize,                 // number of vertices (components)
                          GL_FLOAT,          //
                          GL_FALSE,          // no normalization
                          stride, // no of bytes between each vertice (component)
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          colorSize,                            // no vertices (components)
                          GL_FLOAT,                     //
                          GL_FALSE,                     // no normalization
                          stride,            // no of bytes between each color (component)
                          (void *)(6 * sizeof(float))); // offset to color data in the vbo
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,
                          normalSize,                            // no vertices (components)
                          GL_FLOAT,                     //
                          GL_FALSE,                     // no normalization
                          stride,            // no of bytes between each color (component)
                          (void *)(3 * sizeof(float))); // offset to color data in the vbo
    glEnableVertexAttribArray(2);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  else
   {
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  // Check for OpenGL errors
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
    std::cerr << "OpenGL error: " << error << std::endl;
}

OpenGLView::~OpenGLView()
{
  glDeleteVertexArrays(1, &vao);
}

void OpenGLView::render(SquareMatrix<float, 4> &matrice)
{
  debug(2, "render() entry...");
  glBindVertexArray(vao);
  glUseProgram(shaderProgram);
  unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &matrice[0][0]);
  glDrawArrays(mode, 0, vertices_size);
  debug(2, "render() exit.");
}

// class TypedBodyView

TypedBodyView::TypedBodyView(bool is_3d, TypedBody *typed_body, GLuint vbo, unsigned int shaderProgram, size_t vertices_size, float scale, GLuint mode,
                             std::function<bool()> draw, std::function<void(TypedBodyView *)> modify)
    : OpenGLView(vbo, shaderProgram, vertices_size, mode, is_3d), typed_body(typed_body), scale(scale), draw(draw), modify(modify)
{
}

sm4 TypedBodyView::create_object_transformation(vec2 direction, float angle, float scale)
{
  sm4 translation = {{1.0f, 0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f},
                     {direction[0], direction[1], 0.0f, 1.0f}};
  sm4 rotation = {{std::cos(angle), std::sin(angle), 0.0f, 0.0f},
                  {-std::sin(angle), std::cos(angle), 0.0f, 0.0f},
                  {0.0f, 0.0f, 1.0f, 0.0f},
                  {0.0f, 0.0f, 0.0f, 1.0f}};
  sm4 scaling = {{scale, 0.0f, 0.0f, 0.0f},
                 {0.0f, scale, 0.0f, 0.0f},
                 {0.0f, 0.0f, scale, 0.0f},
                 {0.0f, 0.0f, 0.0f, 1.0f}};

  return translation * rotation * scaling;
}

void TypedBodyView::render(SquareMatrix<float, 4> &world)
{
  debug(2, "render() entry...");
  if (draw())
  {
    modify(this);
    auto transform = world * create_object_transformation(typed_body->get_position(), typed_body->get_angle(), scale);
    OpenGLView::render(transform);
  }
  debug(2, "render() exit.");
}

TypedBody *TypedBodyView::get_typed_body()
{
  return typed_body;
}

void TypedBodyView::set_scale(float scale)
{
  this->scale = scale;
}

// class OpenGLRenderer

void OpenGLRenderer::createVbos()
{
  vbos = new GLuint[vertice_data.size()];
  glGenBuffers(vertice_data.size(), vbos);

  for (size_t i = 0; i < vertice_data.size(); i++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
    glBufferData(GL_ARRAY_BUFFER, vertice_data[i]->size() * sizeof(vec2), vertice_data[i]->data(), GL_STATIC_DRAW);
  }
}


std::vector<float> create_vertices(WavefrontImporter & wi) {
  std::vector<float> vertices;
  Material default_material = {{1.0f, 1.0f, 1.0f}};
  for (Face face : wi.get_faces() ) {
    for (ReferenceGroup group : face.reference_groups ) {
      for (size_t i = 0; i < 3; i++) {
        vertices.push_back( group.vertice[i]);
      }
      for (size_t i = 0; i < 3; i++) {
        vertices.push_back( group.normal[i] );
      }
      if (face.material == nullptr) face.material = &default_material;
      for (size_t i = 0; i < 3; i++) {
        vertices.push_back( face.material->ambient[i]);
      }
    } 
  }
  return vertices;
}


// import given files and create vbos for each object
void OpenGLRenderer::createVbos_3d(std::vector<std::string> files)
{
  std::cout << "Creating 3D objects..." << std::endl;

  for (std::string file : files)
  {
    std::fstream in(file);
    WavefrontImporter wi(in);
    wi.parse();
    std::vector<float> vertices = create_vertices(wi);
    objects.push_back(vertices);
    std::cout << "Created object from " << file << " with " << vertices.size() / 9 << " vertices." << std::endl;
  }

  vbos_3d = new GLuint[objects.size()];
  glGenBuffers(objects.size(), vbos_3d);

  for (size_t i = 0; i < objects.size(); i++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbos_3d[i]);
    glBufferData(GL_ARRAY_BUFFER, objects[i].size() * sizeof(float), objects[i].data(), GL_STATIC_DRAW);
  }
}

void OpenGLRenderer::create(Spaceship *ship, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
  bool is_3d = true;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  debug(4, "create(Spaceship *) entry...");

  views.push_back(std::make_unique<TypedBodyView>(is_3d, ship, vbos_3d[1], shaderProg, objects[0].size()/9, 5.0f, GL_TRIANGLES,
                                                  [ship]() -> bool
                                                  { return !ship->is_in_hyperspace(); }) // only show ship if outside hyperspace
  );
  views.push_back(std::make_unique<TypedBodyView>(is_3d, ship, vbos[1], shaderProg, vertice_data[1]->size(), 1.0f, GL_LINE_LOOP,
                                                  [ship]() -> bool
                                                  { return !ship->is_in_hyperspace() && ship->is_accelerating(); }) // only show flame if accelerating
  );

  debug(4, "create(Spaceship *) exit.");
}

void OpenGLRenderer::create(Saucer *saucer, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
  bool is_3d = true;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  debug(4, "create(Saucer *) entry...");
  float scale = 0.5;
  if (saucer->get_size() == 0)
  {
    scale = 0.25;
  }
  //views.push_back(std::make_unique<TypedBodyView>(is_3d, saucer, vbos[3], shaderProg, vertice_data[3]->size(), scale));
  views.push_back(std::make_unique<TypedBodyView>(is_3d, saucer, vbos_3d[3], shaderProg, objects[0].size()/9, 20.0f, GL_TRIANGLES));
  debug(4, "create(Saucer *) exit.");
}

void OpenGLRenderer::create(Torpedo *torpedo, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
  bool is_3d = true;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  debug(4, "create(Torpedo *) entry...");
  //views.push_back(std::make_unique<TypedBodyView>(is_3d, torpedo, vbos[2], shaderProg, vertice_data[2]->size(), 1.0f));
  views.push_back(std::make_unique<TypedBodyView>(is_3d, torpedo, vbos_3d[2], shaderProg, objects[0].size()/9, 2.0f, GL_TRIANGLES));
  debug(4, "create(Torpedo *) exit.");
}

void OpenGLRenderer::create(Asteroid *asteroid, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
  bool is_3d = true;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  debug(4, "create(Asteroid *) entry...");
  ///GLuint rock_vbo_index = 4 + asteroid->get_rock_type();

  float scale = (asteroid->get_size() == 3 ? 1.0 : (asteroid->get_size() == 2 ? 0.5 : 0.25)) * 30;

  //views.push_back(std::make_unique<TypedBodyView>(is_3d, asteroid, vbos[rock_vbo_index], shaderProg, vertice_data[rock_vbo_index]->size(), scale));
  std::cout << "Vertex Size: " << objects[0].size() << std::endl;
  views.push_back(std::make_unique<TypedBodyView>(is_3d, asteroid, vbos_3d[0], shaderProg, objects[0].size()/9, scale, GL_TRIANGLES));
  debug(4, "create(Asteroid *) exit.");
}

void OpenGLRenderer::create(SpaceshipDebris *debris, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
  bool is_3d = false;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  debug(4, "create(SpaceshipDebris *) entry...");
  views.push_back(std::make_unique<TypedBodyView>(
      is_3d, debris, vbos[10], shaderProg, vertice_data[10]->size(), 0.1f, GL_POINTS,
      []() -> bool
      { return true; },
      [debris](TypedBodyView *view) -> void
      { view->set_scale(0.2f * (SpaceshipDebris::TIME_TO_DELETE - debris->get_time_to_delete())); }));
  debug(4, "create(SpaceshipDebris *) exit.");
}

void OpenGLRenderer::create(Debris *debris, std::vector<std::unique_ptr<TypedBodyView>> &views)
{
  bool is_3d = false;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  debug(4, "create(Debris *) entry...");
  views.push_back(std::make_unique<TypedBodyView>(
      is_3d, debris, vbos[10], shaderProg, vertice_data[10]->size(), 0.1f, GL_POINTS,
      []() -> bool
      { return true; },
      [debris](TypedBodyView *view) -> void
      { view->set_scale(Debris::TIME_TO_DELETE - debris->get_time_to_delete()); }));
  debug(4, "create(Debris *) exit.");
}

void OpenGLRenderer::createSpaceShipView()
{
  bool is_3d = false;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  spaceship_view = std::make_unique<OpenGLView>(vbos[0], shaderProg, vertice_data[0]->size(), GL_LINE_LOOP);
}

void OpenGLRenderer::createDigitViews()
{
  bool is_3d = false;
  auto shaderProg = is_3d ? shaderProgram3D : shaderProgram2D;

  for (size_t i = 0; i < 10; i++)
  {
    digit_views[i] = std::make_unique<OpenGLView>(vbos[11 + i], shaderProg, vertice_data[11 + i]->size(), GL_LINE_STRIP);
  }
}

void OpenGLRenderer::renderFreeShips(sm4 &matrice)
{
  constexpr float FREE_SHIP_X = 128;
  constexpr float FREE_SHIP_Y = 64;
  const float PIf = static_cast<float>(PI);
  vec2 position = {FREE_SHIP_X, FREE_SHIP_Y};
  sm4 rotation = {{std::cos(-PIf / 2.0f), std::sin(-PIf / 2.0f), 0.0f, 0.0f},
                  {-std::sin(-PIf / 2.0f), std::cos(-PIf / 2.0f), 0.0f, 0.0f},
                  {0.0f, 0.0f, 1.0f, 0.0f},
                  {0.0f, 0.0f, 0.0f, 1.0f}};
  for (int i = 0; i < game.get_no_of_ships(); i++)
  {
    sm4 translation = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {0.0f, 0.0f, 1.0f, 0.0f},
                       {position[0], position[1], 0.0f, 1.0f}};
    sm4 render_matrice = matrice * translation * rotation;
    spaceship_view->render(render_matrice);
    position[0] += 20.0;
  }
}

void OpenGLRenderer::renderScore(sm4 &matrice)
{
  constexpr float SCORE_X = 128 - 48;
  constexpr float SCORE_Y = 48 - 4;

  long long score = game.get_score();
  int no_of_digits = 0;
  if (score > 0)
  {
    no_of_digits = std::trunc(std::log10(score)) + 1;
  }

  vec2 position = {SCORE_X + 20.0f * no_of_digits, SCORE_Y};
  do
  {
    int d = score % 10;
    score /= 10;
    sm4 scale_translation = {{4.0f, 0.0f, 0.0f, 0.0f},
                             {0.0f, 4.0f, 0.0f, 0.0f},
                             {0.0f, 0.0f, 1.0f, 0.0f},
                             {position[0], position[1], 0.0f, 1.0f}};
    sm4 render_matrice = matrice * scale_translation;
    digit_views[d]->render(render_matrice);
    no_of_digits--;
    position[0] -= 20;

  } while (no_of_digits > 0);
}

void compile_shader(GLint shader, const char *source)
{
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
  {
    std::cerr << " Shader did not compile." << std::endl;
    char log[512];
    glGetShaderInfoLog(shader, 512, NULL, log);
    std::cerr << log << std::endl;
    throw EXIT_FAILURE;
  }
}

void check_link_status(GLint shader_program)
{
  GLint status;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLint length;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
    GLchar *log = new char[length + 1];
    glGetProgramInfoLog(shader_program, length, &length, &log[0]);
    std::cerr << log << std::endl;
    throw EXIT_FAILURE;
  }
}

void OpenGLRenderer::create_shader_programs()
{

  static const char *vertexShaderSource = "#version 330 core\n"
                                          "layout (location = 0) in vec2 p;\n"
                                          "uniform mat4 model;\n"
                                          "void main()\n"
                                          "{\n"
                                          "   gl_Position = model * vec4(p, 1.0, 1.0);\n"
                                          "}\0";
  static const char *fragmentShaderSource = "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "void main()\n"
                                            "{\n"
                                            "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                            "}\n\0";

  // build and compile vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  compile_shader(vertexShader, vertexShaderSource);

  // build and compiler fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  compile_shader(fragmentShader, fragmentShaderSource);

  // link both shaders
  shaderProgram2D = glCreateProgram();
  glAttachShader(shaderProgram2D, vertexShader);
  glAttachShader(shaderProgram2D, fragmentShader);
  glLinkProgram(shaderProgram2D);
  // check for linking errors
  check_link_status(shaderProgram2D);
}

void OpenGLRenderer::create_shader_programs_3d()
{
  const char *vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "layout (location = 1) in vec3 incolor;\n"
                                   "layout (location = 2) in vec3 innormal;\n"
                                   "out vec3 color;\n"
                                   "out vec4 normal;\n"
                                   "uniform mat4 model;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = model * vec4(position, 1.0);\n"
                                   "color = incolor;\n"
                                   "normal = normalize( model * vec4(innormal, 1.0));\n"
                                   "}\0";

  // direction to light source is hard coded: (0,1,-4)
  // Lambertian shading used for vertices of triangle
  // cause during rasterization colors are interpolated, the result is Gouraud-Shading
  const char *fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 outColor;\n"
                                     "in vec3 color;\n"
                                     "in vec4 normal;\n"
                                     "void main () {\n"
                                     "  outColor = vec4(color * (0.3 + 0.7 * max(0.0, dot(normal, normalize( vec4(0.0, 1.0, -4.0, 0.0))))) , 1.0);\n"
                                     "}\n\0";

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  compile_shader(vertexShader, vertexShaderSource);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  compile_shader(fragmentShader, fragmentShaderSource);

  shaderProgram3D = glCreateProgram();
  glAttachShader(shaderProgram3D, vertexShader);
  glAttachShader(shaderProgram3D, fragmentShader);
  glBindFragDataLocation(shaderProgram3D, 0, "outColor");
  glLinkProgram(shaderProgram3D);
  check_link_status(shaderProgram3D);
}

bool OpenGLRenderer::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    error(std::string("Could not initialize SDL. SDLError: ") + SDL_GetError());
  }
  else
  {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
      error(std::string("Could not create Window. SDLError: ") + SDL_GetError());
    }
    else
    {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

      context = SDL_GL_CreateContext(window);

      GLenum err = glewInit(); // to be called after OpenGL render context is created
      if (GLEW_OK != err)
      {
        error("Could not initialize Glew. Glew error message: ");
        error(glewGetErrorString(err));
      }
      glEnable(GL_DEPTH_TEST); // enable depth-testing
      debug(1, "Using GLEW Version: ");
      debug(1, glewGetString(GLEW_VERSION));

      SDL_GL_SetSwapInterval(1);

      create_shader_programs();
      create_shader_programs_3d();
      createVbos();
      createVbos_3d(std::vector<std::string>{"asteroid.obj", "spaceship.obj", "torpedo.obj", "saucer.obj"});
      createSpaceShipView();
      createDigitViews();
      return true;
    }
  }
  return false;
}

/* tile positions in world coordinates
   used to draw objects seemless between boundary
  +---+---+---+
  | 5 | 7 | 2 |
  +---+---+---+
  | 4 | 0 | 1 |
  +---+---+---+
  | 6 | 8 | 3 |
  +---+---+---+
*/
static vec2 tile_positions[] = {
    {0.0f, 0.0f},
    {1024.0f, 0.0f},
    {1024.0f, 768.0f},
    {1024.0f, -768.0f},
    {-1024.0f, 0.0f},
    {-1024.0f, 768.0f},
    {-1024.0f, -768.0f},
    {0.0f, 768.0f},
    {0.0f, -768.0f}};

void OpenGLRenderer::render()
{
  debug(2, "render() entry...");

  // transformation to canonical view and from left handed to right handed coordinates
  sm4 world_transformation =
      sm4{
          {2.0f / 1024.0f, 0.0f, 0.0f, 0.0f},
          {0.0f, -2.0f / 768.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 2.0f / 1024.0f, 0.0f},
          {-1.0f, 1.0f, -1.0f, 1.0f}};

  sm4 world_transformation_centered = world_transformation;

  if (game.ship_exists())
  {
    Spaceship *ship = game.get_ship();
    vec2 pos_ship = ship->get_position();
   
    /* sm4 center_transformation = sm4{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {512.0f - pos_ship[0], 384.0f - pos_ship[1], 2.0f / 1024.0f, 0.0f},
        {-1.0f, 1.0f, -1.0f, 1.0f}};

    world_transformation_centered = world_transformation * center_transformation; */

    sm4 center_ship_transform {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {-2 * (pos_ship[0] - 512.f) / 1024.0f, 2 * (pos_ship[1] - 368.f) / 768.0f, 0.0f, 1.0f}
    };
    world_transformation_centered = world_transformation * center_ship_transform;
  }

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  debug(2, "remove views for deleted objects");

  // remove all views for typed bodies that have to be deleted
  std::erase_if(views, [](std::unique_ptr<TypedBodyView> &view)
           { return view->get_typed_body()->is_marked_for_deletion(); });

  auto new_bodies = game.get_physics().get_recently_added_bodies();
  for (Body2df *body : new_bodies)
  {
    assert(body != nullptr);
    TypedBody *typed_body = static_cast<TypedBody *>(body);
    auto type = typed_body->get_type();
    if (type == BodyType::spaceship)
    {
      create(static_cast<Spaceship *>(typed_body), views);
    }
    else if (type == BodyType::torpedo)
    {
      create(static_cast<Torpedo *>(typed_body), views);
    }
    else if (type == BodyType::asteroid)
    {
      create(static_cast<Asteroid *>(typed_body), views);
    }
    else if (type == BodyType::saucer)
    {
      create(static_cast<Saucer *>(typed_body), views);
    }
    else if (type == BodyType::spaceship_debris)
    {
      create(static_cast<SpaceshipDebris *>(typed_body), views);
    }
    else if (type == BodyType::debris)
    {
      create(static_cast<Debris *>(typed_body), views);
    }
  }

  debug(2, "render all views");
  for (auto &view : views)
  {
    view->render(world_transformation_centered);
  }

  glDisable(GL_DEPTH_TEST);
  renderFreeShips(world_transformation);
  renderScore(world_transformation);
  glEnable(GL_DEPTH_TEST);

  SDL_GL_SwapWindow(window);
  debug(2, "render() exit.");
}

void OpenGLRenderer::exit()
{
  views.clear();
  glDeleteBuffers(vertice_data.size(), vbos);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
