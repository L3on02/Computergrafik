#include "timer.h"
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

ActionCounter::ActionCounter(float time, std::function<void(void)> action)
  : Counter(time), action(action) { }
  
void ActionCounter::tick(float seconds) {
  if ( get_time() > 0.0f) {
     Counter::tick(seconds);
     if ( get_time() <= 0.0f) {
       action();
     }
  }
}


void Timer::reset() {
  start = SDL_GetTicks64();
}

void Timer::tick_and_delay(float tick_time) {
  end = SDL_GetTicks64();
  Uint64 elapse = end - start;
  SDL_Delay(abs(1000.0f * tick_time - elapse));
  tick(tick_time);
}

  
void Timer::tick(float tick_time) {
  time += tick_time;
}
