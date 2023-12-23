#ifndef OPENGL_RENDERER
#define OPENGL_RENDERER


#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "matrix.h"
#include "physics.h"
#include "game.h"
#include "renderer.h"
#include "debug.h"
#include <array>
#include <vector>
#include <memory>

// stores information on how to render a specific vertex buffer (vbo)
// the vob's layout used by the shaderProgram is hard coded into the render() method.
class OpenGLView {
protected:
  unsigned int shaderProgram;
  size_t vertices_size;
  GLuint mode;
  GLuint vao;
public:
  OpenGLView(GLuint vbo, unsigned int shaderProgram, size_t vertices_size, GLuint mode = GL_LINE_LOOP);  

  ~OpenGLView();
    
  void render( SquareMatrix<float,4> & matrice);  
};


class TypedBodyView : public OpenGLView {
  TypedBody * typed_body;    // the body that is rendered by this view
  float scale;
  std::function<bool()> draw; // view is rendered iff draw() returns true
  std::function<void(TypedBodyView *)> modify; // a callback which my change this TypedBodyView, for instance, for animations
  sm4 create_object_transformation(vec2 direction, float angle, float scale);
public:
  TypedBodyView(TypedBody * typed_body, GLuint vbo, unsigned int shaderProgram, size_t vertices_size, float scale = 1.0f, GLuint mode = GL_LINE_LOOP,
               std::function<bool()> draw = []() -> bool {return true;},
               std::function<void(TypedBodyView *)> modify = [](TypedBodyView *) -> void {});

  // returns a 4 x 4 transformation matrice that rotates an object counter clockwise by the given angle in the x/y plane,
  // scales it, and moves it to the given direction 
 
  void render( SquareMatrix<float,4> & world) ;
  
 TypedBody * get_typed_body();

 void set_scale(float scale);
 
};


// OpenGLRenderer is responsible for creating and opening a window for the Asteroid-Game, when init() is called.
// Each time render() is called, it draws all visible game objects, score, ...
// exit() frees view resources and closes the window
class OpenGLRenderer : public Renderer {
  std::string title;
  int window_width;
  int window_height;
  SDL_Window * window = nullptr;
  SDL_GLContext context;
  unsigned int shaderProgram;
  std::vector< std::unique_ptr<TypedBodyView > > views;
  GLuint * vbos;
  std::unique_ptr<OpenGLView> spaceship_view;
  std::array< std::unique_ptr<OpenGLView>, 10> digit_views;
  void createVbos();
  void createSpaceShipView();
  void createDigitViews();
  void create(Spaceship * ship, std::vector< std::unique_ptr<TypedBodyView> > & views); 
  void create(Torpedo * torpedo, std::vector< std::unique_ptr<TypedBodyView> > & views);
  void create(Asteroid * asteroid, std::vector< std::unique_ptr<TypedBodyView> > & views);
  void create(Saucer * saucer, std::vector< std::unique_ptr<TypedBodyView> > & views);
  void create(SpaceshipDebris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views);
  void create(Debris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views);
  void renderFreeShips(sm4 & matrice);
  void renderScore(sm4 & matrice);
  void create_shader_programs();
public:
  OpenGLRenderer(Game & game, std::string title, int window_width = 1024, int window_height = 768)
    : Renderer(game), title(title), window_width(window_width), window_height(window_height) { }
  
  ~OpenGLRenderer() { delete [] vbos; }
  
  virtual bool init();
  
  virtual void render();
  
  virtual void exit(); 
  
};

#endif
