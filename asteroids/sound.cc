#include "sound.h"

Effect::Effect(std::span<SoundId> wave_ids, float interval_between_sounds, float duration)
 : interval_between_sounds(interval_between_sounds), duration(duration) {
  for (SoundId id : wave_ids) {
    this->waves.push_back(id);
  }
}

void Effect::set_interval_between_sound(float interval_between_sounds) {
  this->interval_between_sounds = interval_between_sounds;
}
  
void Effect::cancel() {
  this->duration = -1.0;
}

void Effect::switch_on() {
  on = true;
}

void Effect::switch_off() {
  on = false;
}

Sound::~Sound() {
  for (Mix_Chunk * chunk : sounds) {
    Mix_FreeChunk(chunk);
  }
}

void Sound::init() {
  SDL_Init(SDL_INIT_AUDIO);
  int result;
  if( (result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) ) < 0 ) {
      std::cerr << "Unable to open audio: " << SDL_GetError() << std::endl;
  }
  if( (result = Mix_AllocateChannels(8)) < 0 ) {
      std::cerr << "Unable to allocate mixing channels: " << SDL_GetError() << std::endl;
  }
  for (size_t i = 0; i < std::span{sounds}.size(); i++) {
    if ( (sounds[i] = Mix_LoadWAV(file_names[i])) == nullptr) {
      std::cerr << "Unable to load '" << file_names[i] << "' : " << SDL_GetError() << std::endl;      
    }
  }
}

void Sound::play_immediate(SoundId sound_id) {
  Mix_PlayChannel(-1, sounds[sound_id], 0);
}

void Sound::play_looped(SoundId sound_id, int loops) {
  Mix_PlayChannelTimed(-1, sounds[sound_id], loops, -1);
}

void Sound::add_effect(Effect * effect) {
  effects.push_back(effect);  
}

void Sound::erase_effect(Effect * effect) {
  std::erase( effects, effect);
}

void Sound::tick(float seconds) {
  for (Effect * effect : effects) {
    effect->current_duration += seconds;
    effect->current_interval += seconds;
    if (effect->on && effect->current_interval > effect->interval_between_sounds) {
        effect->current_interval = 0.0;
        effect->current_wave = (effect->current_wave + 1) % effect->waves.size();
        play_immediate(effect->waves[effect->current_wave]);
    }
  }

}

