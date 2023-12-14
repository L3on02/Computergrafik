#include "timer.h"
#include "debug.h"
#include <thread>
#include <iostream>

Counter::Counter(float time) : time(time) { }

float Counter::get_time() const {
  return time;
}

void Counter::set_time(float time) {
  this->time = time;
}

void Counter::tick(float seconds) {
  if (time > 0.0) {
    time -= seconds;
  }
}

void Timer::reset() {
  start = SDL_GetTicks64();
}

void Timer::tick_and_delay(float tick_time) {
  debug(4, "tick_and_delay() entry...");
  end = SDL_GetTicks64();
  Uint64 elapse = end - start;
  auto delay = 1000.0f * tick_time - static_cast<float>(elapse);
  if ( delay > 0.0f) {
    SDL_Delay(delay);
  }
  tick(tick_time);
  debug(4, "tick_and_delay() exit.");
}

  
void Timer::tick(float tick_time) {
  time += tick_time;
}
