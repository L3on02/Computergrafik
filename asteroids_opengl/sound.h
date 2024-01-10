#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "timer.h"
#include <iostream>
#include <vector>
#include <span>

typedef size_t SoundId;

class Sound;

class Effect {
  std::vector<SoundId> waves;
  float interval_between_sounds;
  float duration;
  size_t current_wave = 0; 
  float current_interval = 0.0f;
  float current_duration = 0.0f;
  bool on = false;
public:  
  Effect(std::span<SoundId> wave_ids, float interval_between_sounds, float duration);

  void set_interval_between_sound(float interval_between_sounds);  
  void cancel();
  void switch_on();
  void switch_off();
  friend Sound;
};

class Sound {
  const char * file_names[10] = {"../sound/fire.wav",
                                 "../sound/extraShip.wav",
                                 "../sound/bangSmall.wav",
                                 "../sound/bangMedium.wav",
                                 "../sound/bangLarge.wav",
                                 "../sound/beat1.wav",
                                 "../sound/beat2.wav",
                                 "../sound/saucerSmall.wav",
                                 "../sound/saucerBig.wav",
                                 "../sound/thrust.wav" };
  Mix_Chunk * sounds[10];
  std::vector<Effect *> effects;
public:
  static constexpr size_t FIRE = 0;
  static constexpr size_t EXTRA_SHIP = 1;
  static constexpr size_t BANG_SMALL = 2;
  static constexpr size_t BANG_MEDIUM = 3;
  static constexpr size_t BANG_LARGE = 4;
  static constexpr size_t BEAT1 = 5;
  static constexpr size_t BEAT2 = 6;
  static constexpr size_t SAUCER_SMALL = 7;
  static constexpr size_t SAUCER_BIG = 8;
  static constexpr size_t THRUST = 9;

  ~Sound();
  
  void init();
  
  void play_immediate(SoundId sound_id);
  
  void play_looped(SoundId sound_id, int loops);
  
  void add_effect(Effect * effect);
  
  void erase_effect(Effect * effect);
  
  void tick(float seconds);
};

#endif
