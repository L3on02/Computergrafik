#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game.h"

class GameController {
protected:
  Game & game;
  bool quit = false;
public:
  GameController(Game & game) : game(game) {  }
  
  virtual void do_user_interactions() = 0;
  
  virtual void do_game_events() = 0;

  bool exit_game() const {
    return quit;
  }
  
};

#endif
