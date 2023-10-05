#include "sdl2_game_controller.h"

#include <vector>

SDL2GameController::SDL2GameController(Game & game) : GameController(game) {
  if( SDL_Init( SDL_INIT_EVENTS ) < 0 ) {
    std::cout << "SDL_Init: could not init event subsystem" << std::endl;
  }
  sound.init();
  set_fps(60);
  sound.add_effect( &backgroundSound );
}

float SDL2GameController::get_tick_time() const {
  return tick_time;
}

void SDL2GameController::set_fps(int fps) {
  tick_time = 1.0f / fps;
  this->fps = fps;
}


void SDL2GameController::do_user_interactions() {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);
 
  SDL_Event e;
  while ( SDL_PollEvent( &e ) ) {
    if (e.type == SDL_QUIT ) {
      quit = true;
    }
  }

  if (! quit) {
    game.tick(tick_time);
    sound.tick(tick_time);

    if ( keys[SDL_SCANCODE_LEFT] ) {
      game.get_ship().turn_left(tick_time);
    }
    if ( keys[SDL_SCANCODE_RIGHT] ) {
       game.get_ship().turn_right(tick_time);
    }
    if ( keys[SDL_SCANCODE_UP] ) {
      game.accelerate_ship(tick_time);
    }
    if ( keys[SDL_SCANCODE_D] ) {
      game.ship_shoots();
    }
    if ( keys[SDL_SCANCODE_SPACE] ) {
      game.hyperspace();
    }
  }
}

void SDL2GameController::do_game_events(){
  if (! quit) {
    for (GameEvent event : game.get_game_events() ) {
      switch ( event ){
        case GameEvent::new_ship_spawned:
          backgroundSound.switch_on();
          break;
        case GameEvent::end_of_level:
          backgroundSound.switch_off();          
          break;
        case GameEvent::next_level_started:
          backgroundSound.set_interval_between_sound(MAX_DISTANCE_BETWEEN_BEATS);
          backgroundSound.switch_on();
          break;
        case GameEvent::torpedo_fired:
          sound.play_immediate(Sound::FIRE);
          break;
        case GameEvent::small_asteroid_destroyed:
          sound.play_immediate(Sound::BANG_SMALL);
          break;
        case GameEvent::medium_asteroid_destroyed:
          sound.play_immediate(Sound::BANG_MEDIUM);
          break;
        case GameEvent::ship_destroyed:
          backgroundSound.switch_off();
          sound.play_immediate(Sound::BANG_MEDIUM);
          break;
        case GameEvent::large_asteroid_destroyed:
        case GameEvent::big_saucer_destroyed:
        case GameEvent::small_saucer_destroyed:
          sound.play_immediate(Sound::BANG_LARGE);
          break;
        case GameEvent::ship_thrust:
          sound.play_immediate(Sound::THRUST);
          break;
        case GameEvent::extra_ship_gained:
          sound.play_looped(Sound::EXTRA_SHIP, 10);
          break;
        default: break;
      }
    }    
    if ( game.get_time_since_start_of_level() > 40.0 ) {
      backgroundSound.set_interval_between_sound(0.25f * MAX_DISTANCE_BETWEEN_BEATS);
    } else if ( game.get_time_since_start_of_level() > 30.0 ) {
      backgroundSound.set_interval_between_sound(0.5f * MAX_DISTANCE_BETWEEN_BEATS);
    } else if ( game.get_time_since_start_of_level() > 15.0 ) {
      backgroundSound.set_interval_between_sound(0.75f * MAX_DISTANCE_BETWEEN_BEATS);
    }
    game.get_game_events().clear();
  }
}
