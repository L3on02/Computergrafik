#include "opengl_renderer.h"
#include <cassert>
#include <span>
#include <utility>


// geometric data as in original game and game coordinates
std::vector<vec2> spaceship = {
  vec2{-6.0f,  3.0f},
  vec2{-6.0f, -3.0f},
  vec2{-10.0f, -6.0f},
  vec2{14.0f,  0.0f},
  vec2{-10.0f,  6.0f},
  vec2{ -6.0f,  3.0f}
};

std::vector<vec2> flame = { 
  vec2{-6, 3},
  vec2{-12, 0},
  vec2{-6, -3}
};

std::vector<vec2> torpedo_points = { 
  vec2{0, 0},
  vec2{0, 1}
};

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
  vec2{-40, 6}
};

std::vector<vec2> asteroid_1 = {
  vec2{ 0, -12},
  vec2{16, -24},
  vec2{32, -12},
  vec2{24, 0},
  vec2{32, 12},
  vec2{8, 24}, 
  vec2{-16, 24}, 
  vec2{-32, 12}, 
  vec2{-32, -12}, 
  vec2{-16, -24},
  vec2{0, -12}
};

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
  vec2{-32, -6}
};

std::vector<vec2> asteroid_4 = {
  vec2{8,0}, 
  vec2{32,-6}, 
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
  vec2{8, 0}
};

std::vector<vec2> spaceship_debris = {
  vec2{-2, -1}, 
  vec2{-10, 7}, 
  vec2{3, 1}, 
  vec2{7, 8},
  vec2{0, 3}, 
  vec2{6, 1},
  vec2{3, -1}, 
  vec2{ -5, -7},
  vec2{0, -4}, 
  vec2{-6, -6},
  vec2{-2, 2}, 
  vec2{2, 5}
};
    
std::vector<vec2> spaceship_debris_direction = {
 vec2{-40, -23},
 vec2{50, 15},
 vec2{0, 45},
 vec2{60, -15}, 
 vec2{10, -52}, 
 vec2{-40, 30}
};

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
 vec2{32, -8}
};
        
std::vector<vec2> digit_0 = { {0,-8}, {4,-8}, {4,0}, {0,0}, {0, -8} };
std::vector<vec2> digit_1 = { {4,0}, {4,-8} };
std::vector<vec2> digit_2 = { {0,-8}, {4,-8}, {4,-4}, {0,-4}, {0,0}, {4,0}  };
std::vector<vec2> digit_3 = { {0,0}, {4, 0}, {4,-4}, {0,-4}, {4,-4}, {4, -8}, {0, -8}  };
std::vector<vec2> digit_4 = { {4,0}, {4,-8}, {4,-4}, {0,-4}, {0,-8}  };
std::vector<vec2> digit_5 = { {0,0}, {4,0}, {4,-4}, {0,-4}, {0,-8}, {4, -8}  };
std::vector<vec2> digit_6 = { {0,-8}, {0,0}, {4,0}, {4,-4}, {0,-4} };
std::vector<vec2> digit_7 = { {0,-8}, {4,-8}, {4,0} };
std::vector<vec2> digit_8 = { {0,-8}, {4,-8}, {4,0}, {0,0}, {0,-8}, {0, -4}, {4, -4} };
std::vector<vec2> digit_9 = { {4, 0}, {4,-8}, {0,-8}, {0, -4}, {4, -4} };
       
std::vector< std::vector<vec2> * > vertice_data = {
  &spaceship, &flame,
  &torpedo_points, &saucer_points,
  &asteroid_1, &asteroid_2, &asteroid_3, &asteroid_4,
  &spaceship_debris, &spaceship_debris_direction,
  &debris_points,
  &digit_0, &digit_1, &digit_2, &digit_3, &digit_4, &digit_5, &digit_6, &digit_7, &digit_8, &digit_9 };                                  


// class OpenGLView

  OpenGLView::OpenGLView(GLuint vbo, unsigned int shaderProgram, size_t vertices_size, GLuint mode)
    : shaderProgram(shaderProgram), vertices_size(vertices_size), mode(mode) {
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  OpenGLView::~OpenGLView() {
   glDeleteVertexArrays(1, &vao);
  }

  void OpenGLView::render( SquareMatrix<float,4> & matrice) {
    debug(2, "render() entry...");
    glBindVertexArray(vao);
    glUseProgram(shaderProgram);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &matrice[0][0] );
    glDrawArrays(mode, 0, vertices_size );
    debug(2, "render() exit.");
  }

// class TypedBodyView

  TypedBodyView::TypedBodyView(TypedBody * typed_body, GLuint vbo, unsigned int shaderProgram, size_t vertices_size, float scale, GLuint mode,
               std::function<bool()> draw, std::function<void(TypedBodyView *)> modify)
        : OpenGLView(vbo, shaderProgram, vertices_size, mode),  typed_body(typed_body), scale(scale), draw(draw), modify(modify) {
  }
  
  sm4 TypedBodyView::create_object_transformation(vec2 direction, float angle, float scale) {
    sm4  translation= { {1.0f,        0.0f,         0.0f, 0.0f},
                                    {0.0f,        1.0f,         0.0f, 0.0f},
                                    {direction[0], direction[1],  1.0f, 0.0f},
                                    {0.0f,        0.0f,         0.0f, 1.0f} };

    sm4 rotation = { { std::cos(angle),  std::sin(angle), 0.0f, 0.0f},
                                 {-std::sin(angle),  std::cos(angle), 0.0f, 0.0f},
                                 { 0.0f,             0.0f,            1.0f, 0.0f},
                                 { 0.0f,             0.0f,            0.0f, 1.0f}
                               };
    sm4  scaling = { { scale,    0.0f, 0.0f,  0.0f},
                                 {  0.0f,   scale, 0.0f,  0.0f},
                                 {  0.0f,    0.0f, 1.0f,  0.0f},
                                 {  0.0f,    0.0f, 0.0f,  1.0f}
                               };                                 

    return translation * rotation * scaling;
  }

  void TypedBodyView::render( SquareMatrix<float,4> & world) {
    debug(2, "render() entry...");
    if ( draw() ) {
      modify(this);
      auto transform = world * create_object_transformation(typed_body->get_position(), typed_body->get_angle(), scale);
      OpenGLView::render(transform);
    }
    debug(2, "render() exit.");
  }
  
 TypedBody * TypedBodyView::get_typed_body() {
   return typed_body;
 }

 void TypedBodyView::set_scale(float scale) {
   this->scale = scale;
 }

// class OpenGLRenderer

void OpenGLRenderer::createVbos() {
 vbos = new GLuint[vertice_data.size()];
 glGenBuffers(vertice_data.size(), vbos);

 for (size_t i = 0; i < vertice_data.size(); i++) {
   glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
   glBufferData(GL_ARRAY_BUFFER, vertice_data[i]->size() * sizeof( vec2 ), vertice_data[i]->data(), GL_STATIC_DRAW);
 }
}

void OpenGLRenderer::create(Spaceship * ship, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  debug(4, "create(Spaceship *) entry...");

  views.push_back(std::make_unique<TypedBodyView>(ship, vbos[0], shaderProgram, vertice_data[0]->size(), 1.0f, GL_LINE_LOOP,
                  [ship]() -> bool {return ! ship->is_in_hyperspace();}) // only show ship if outside hyperspace
                 );   
  views.push_back(std::make_unique<TypedBodyView>(ship, vbos[1], shaderProgram, vertice_data[1]->size(), 1.0f, GL_LINE_LOOP,
                  [ship]() -> bool {return ! ship->is_in_hyperspace() && ship->is_accelerating();}) // only show flame if accelerating
                 );   
  
  debug(4, "create(Spaceship *) exit.");
}

void OpenGLRenderer::create(Saucer * saucer, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  debug(4, "create(Saucer *) entry...");
  float scale = 0.5;
  if ( saucer->get_size() == 0 ) {
    scale = 0.25;
  }
  views.push_back(std::make_unique<TypedBodyView>(saucer, vbos[3], shaderProgram, vertice_data[3]->size(), scale));   
  debug(4, "create(Saucer *) exit.");
}


void OpenGLRenderer::create(Torpedo * torpedo, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  debug(4, "create(Torpedo *) entry...");
  views.push_back(std::make_unique<TypedBodyView>(torpedo, vbos[2], shaderProgram, vertice_data[2]->size(), 1.0f)); 
  debug(4, "create(Torpedo *) exit.");
}

void OpenGLRenderer::create(Asteroid * asteroid, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  debug(4, "create(Asteroid *) entry...");
  GLuint rock_vbo_index = 4 +  asteroid->get_rock_type();

  float scale = (asteroid->get_size() == 3 ? 1.0 : ( asteroid->get_size() == 2 ? 0.5 : 0.25 ));
 
  views.push_back(std::make_unique<TypedBodyView>(asteroid, vbos[rock_vbo_index], shaderProgram, vertice_data[rock_vbo_index]->size(), scale)); 
  debug(4, "create(Torpedo *) exit.");
}

void OpenGLRenderer::create(SpaceshipDebris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  debug(4, "create(SpaceshipDebris *) entry...");
  views.push_back(std::make_unique<TypedBodyView>(debris, vbos[10], shaderProgram, vertice_data[10]->size(), 0.1f, GL_POINTS,
            []() -> bool {return true;},
            [debris](TypedBodyView * view) -> void { view->set_scale( 0.2f * (SpaceshipDebris::TIME_TO_DELETE - debris->get_time_to_delete()));}));   
  debug(4, "create(SpaceshipDebris *) exit.");
}

void OpenGLRenderer::create(Debris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  debug(4, "create(Debris *) entry...");
  views.push_back(std::make_unique<TypedBodyView>(debris, vbos[10], shaderProgram, vertice_data[10]->size(), 0.1f, GL_POINTS,
            []() -> bool {return true;},
            [debris](TypedBodyView * view) -> void { view->set_scale(Debris::TIME_TO_DELETE - debris->get_time_to_delete());}));   
  debug(4, "create(Debris *) exit.");
}

void OpenGLRenderer::createSpaceShipView() {
  spaceship_view = std::make_unique<OpenGLView>(vbos[0], shaderProgram, vertice_data[0]->size(), GL_LINE_LOOP);
}

void OpenGLRenderer::createDigitViews() {
  for (size_t i = 0; i < 10; i++ ) {
    digit_views[i] = std::make_unique<OpenGLView>(vbos[11 + i], shaderProgram, vertice_data[11 + i]->size(), GL_LINE_STRIP);
  }
}


void OpenGLRenderer::renderFreeShips(sm4 & matrice) {
  constexpr float FREE_SHIP_X = 128;
  constexpr float FREE_SHIP_Y = 64;
  const float PIf = static_cast<float> ( PI );
  vec2 position = {FREE_SHIP_X, FREE_SHIP_Y};
  sm4 rotation = {   { std::cos(-PIf / 2.0f),  std::sin(-PIf / 2.0f), 0.0f, 0.0f},
                                 {-std::sin(-PIf / 2.0f),  std::cos(-PIf / 2.0f), 0.0f, 0.0f},
                                 { 0.0f,                 0.0f,                1.0f, 0.0f},
                                 { 0.0f,                 0.0f,                0.0f, 1.0f}
                               };
  for (int i = 0; i < game.get_no_of_ships(); i++) {
    sm4  translation= { {1.0f,        0.0f,         0.0f, 0.0f},
                                    {0.0f,        1.0f,         0.0f, 0.0f},
                                    {position[0], position[1],  1.0f, 0.0f},
                                    {0.0f,        0.0f,         0.0f, 1.0f} };
    sm4 render_matrice = matrice * translation * rotation;
    spaceship_view->render( render_matrice );
    position[0] += 20.0;
  }
}

void OpenGLRenderer::renderScore(sm4 & matrice) {
  constexpr float SCORE_X = 128 - 48;
  constexpr float SCORE_Y = 48 - 4;
  

  long long score = game.get_score();
  int no_of_digits = 0;
  if (score > 0) {
    no_of_digits = std::trunc( std::log10( score ) ) + 1;
  }

  vec2 position = {SCORE_X + 20.0f * no_of_digits,  SCORE_Y};  
  do {
    int d = score % 10;
    score /= 10;
    sm4 scale_translation= { {4.0f,        0.0f,         0.0f, 0.0f},
                                         {0.0f,        4.0f,         0.0f, 0.0f},
                                         {position[0], position[1],  1.0f, 0.0f},
                                         {0.0f,        0.0f,         0.0f, 1.0f} };
    sm4 render_matrice = matrice * scale_translation;
    digit_views[d]->render( render_matrice );
    no_of_digits--;
    position[0] -= 20;

  } while (no_of_digits > 0);
}


void OpenGLRenderer::create_shader_programs() {

static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 p;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(p, 1.0, 1.0);\n"
    "}\0";
static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

    // build and compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        error( std::string("vertex shader compilation failed") + infoLog);
    }
    // build and compiler fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        error( std::string("fragment shader compilation failed") + infoLog);
    }
    

    // link both shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        error( std::string("linking shader programs failed") + infoLog);
    }
}



bool OpenGLRenderer::init() {
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    error( std::string("Could not initialize SDL. SDLError: ") + SDL_GetError() );
  } else {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if( window == nullptr ) {
      error( std::string("Could not create Window. SDLError: ") + SDL_GetError() );
    } else {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

      context = SDL_GL_CreateContext(window);
      
      GLenum err = glewInit(); // to be called after OpenGL render context is created
      if (GLEW_OK != err) {
        error( "Could not initialize Glew. Glew error message: " );
        error( glewGetErrorString(err) );
      }
      debug(1, "Using GLEW Version: ");
      debug(1, glewGetString(GLEW_VERSION) );

      SDL_GL_SetSwapInterval(1);

      create_shader_programs();
      createVbos();
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
static vec2 tile_positions [] = {
                         {0.0f, 0.0f},
                         {1024.0f, 0.0f},
                         {1024.0f, 768.0f},
                         {1024.0f, -768.0f},
                         {-1024.0f, 0.0f},
                         {-1024.0f, 768.0f},
                         {-1024.0f, -768.0f},
                         {0.0f, 768.0f},
                         {0.0f, -768.0f} };

void OpenGLRenderer::render() {
  debug(2, "render() entry...");

  // transformation to canonical view and from left handed to right handed coordinates
  sm4 world_transformation =
                         sm4{
                           { 2.0f / 1024.0f,           0.0f, 0.0f,  0.0f},
                           {       0.0f,     -2.0f / 768.0f, 0.0f,  0.0f}, // (negative, because we have a left handed world coord. system)
                           {       0.0f,               0.0f, 1.0f,  0.0f},
                           {      -1.0f,               1.0f, 0.0f,  1.0f}
                         };
                                                 
  glClearColor ( 0.0, 0.0, 0.0, 1.0 );
  glClear ( GL_COLOR_BUFFER_BIT );
  
  debug(2, "remove views for deleted objects");

  // remove all views for typed bodies that have to be deleted 
  erase_if(views, []( std::unique_ptr<TypedBodyView> & view) { return view->get_typed_body()->is_marked_for_deletion();}); 

  auto new_bodies = game.get_physics().get_recently_added_bodies();
  for (Body2df * body : new_bodies) {
    assert(body != nullptr);
    TypedBody * typed_body = static_cast<TypedBody *>(body);
    auto type = typed_body->get_type();
    if (type == BodyType::spaceship) {
      create( static_cast<Spaceship *>(typed_body), views );
    } else if (type == BodyType::torpedo ) {
      create( static_cast<Torpedo *>(typed_body), views );
    } else  if (type == BodyType::asteroid) {
      create( static_cast<Asteroid *>(typed_body), views );
    } else if (type == BodyType::saucer) {
      create( static_cast<Saucer *>(typed_body), views );
    } else if (type == BodyType::spaceship_debris ) {
      create( static_cast<SpaceshipDebris *>(typed_body), views );
    } else if (type == BodyType::debris) {
      create( static_cast<Debris *>(typed_body), views );
    }
  }

  debug(2, "render all views");
  for (auto & view : views) {
    view->render( world_transformation );
  }
  
  renderFreeShips(world_transformation);
  renderScore(world_transformation);

  SDL_GL_SwapWindow(window);
  debug(2, "render() exit.");
}

void OpenGLRenderer::exit() {
  views.clear();
  glDeleteBuffers(vertice_data.size(), vbos);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow( window );
  SDL_Quit();
}
 
