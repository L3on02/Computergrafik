#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <SDL2/SDL.h>

#ifndef SDL_GetTicks64
#define SDL_GetTicks64 SDL_GetTicks
#endif

class Counter {
  float time;
public:
  Counter(float time = 0.0f);
  float get_time() const;
  void set_time(float time);
  void tick(float seconds);
};

class ActionCounter : Counter {
  std::function<void(void)> action;
public:
  ActionCounter(float time = 0.0f, std::function<void(void)> action = []( ) -> void { });
  void tick(float seconds);
};

class Timer {
  Uint64 start = SDL_GetTicks64();
  Uint64 end;
  float time = 0.0;
public:
  void tick_and_delay(float tick_time);
  
  void tick(float tick_time);
  
  void reset();
};

#endif
