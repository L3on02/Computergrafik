#ifndef SDL2_RENDERER
#define SDL2_RENDERER

#include <SDL2/SDL.h>
#include <iostream>
#include "physics.h"
#include "game.h"
#include "renderer.h"

// SDL2Renderer is responsible for creating and opening a window for the Asteroid-Game, when init() is called.
// Each time render() is called, it draws all visible game objects, score, ...
// exit() frees view resources and closes the window
class SDL2Renderer : public Renderer {
  std::string title;
  int window_width;
  int window_height;
  SDL_Window * window = nullptr;
  SDL_Surface * screenSurface = nullptr;
  SDL_Renderer * renderer = nullptr;

  // render methods for the specific game objects, score, and free ships
  void renderSpaceship(Vector2df position, float angle);
  void render(Spaceship * ship); 
  void render(Torpedo * torpedo);
  void render(Asteroid * asteroid);
  void render(SpaceshipDebris * debris);
  void render(Debris * debris); 
  void render(Saucer * saucer);
  void renderFreeShips();
  void renderScore();
public:
  SDL2Renderer(Game & game, std::string title, int window_width = 1024, int window_height = 768)
    : Renderer(game), title(title), window_width(window_width), window_height(window_height) { }
  
  virtual bool init();
  
  virtual void render();
  
  virtual void exit(); 
  
};

#endif
