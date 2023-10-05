#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"

// specifices essential methods for a minimal game renderer
class Renderer {
protected:
  Game & game;  
public:
  Renderer(Game & game) : game(game) { }
  virtual bool init() = 0;
  virtual void render() = 0;
  virtual void exit() = 0;
};

#endif
