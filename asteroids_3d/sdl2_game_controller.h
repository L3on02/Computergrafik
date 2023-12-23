#ifndef SDL2_GAME_CONTROLLER_H
#define SDL2_GAME_CONTROLLER_H

#include <SDL2/SDL.h>
#include "game_controller.h"
#include "timer.h"
#include "sound.h"
#include <span>

class SDL2GameController : public GameController {
  static constexpr float MAX_DISTANCE_BETWEEN_BEATS = 10.0f / 12.0f;
  SoundId beats[2] = {Sound::BEAT1, Sound::BEAT2};
  float tick_time;
  int fps;
  Sound sound;
  Effect backgroundSound = Effect( std::span{beats}, MAX_DISTANCE_BETWEEN_BEATS, 10.0f);
public:
  SDL2GameController(Game & game);
  virtual void do_user_interactions();
  virtual void do_game_events();
  float get_tick_time() const;
  void set_fps(int fps);
};

#endif
