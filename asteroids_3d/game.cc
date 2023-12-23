#include "game.h"
#include "debug.h"
#include <iostream>
#include <algorithm>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = (SCREEN_WIDTH * 3) / 4;

void displacement_fix(Body2df * body, float seconds) {
  float x = body->get_position()[0];
  float y = body->get_position()[1];
  vec2 new_position = body->get_position();
  
  if ( x < 0 ) {
    new_position[0] = SCREEN_WIDTH;
  }
  if ( x > SCREEN_WIDTH ) {
    new_position[0] = 0;
  }
  if ( y < 0 ) {
    new_position[1] = SCREEN_HEIGHT;
  }
  if ( y > SCREEN_HEIGHT ) {
    new_position[1] = 0;
  }
  
  body->set_position(new_position);
}

Asteroid::Asteroid(short size)
  : TypedBody( BodyType::asteroid,
               Body2df{ BoundingVolume2df{ vec2{ 128.0f + 768.0f * dis(gen), 64.0f + 640.0f * dis(gen) }, size * 11.0f },
                         vec2{ 0.5f - dis(gen), 0.5f - dis(gen) },
                         348.0, 0.0, 0.0, displacement_fix } ),
    size(size),
    rock_type( std::trunc(4 * dis(gen)) )
  {

    velocity /= velocity.length();
    if (size == 3) { /* 5 - 10 s to cross the screen */
      velocity *= 768.0f / 10.0f +  768.0f / 10.0f * dis(gen);
    } else if (size == 2) { /* 4 - 8s */
      velocity *= 768.0f / 8.0f +  768.0f / 8.0f * dis(gen);
    } else if (size == 1) { /* 3 - 6s */
      velocity *= 768.0f / 6.0f +  768.0f / 6.0f * dis(gen);
    }

  }


Asteroid::Asteroid(short size, vec2 position ) : Asteroid(size) {
  set_position(position);
}
  
short Asteroid::get_size() const {
  return size;
}

short Asteroid::get_rock_type() const {
  return rock_type;
}

bool Spaceship::shoot(Physics2df & physics) {
  if (shoot_cooldown.get_time() <= 0.0 && ! is_marked_for_deletion() && ! in_hyperspace) {
    if ( no_of_torpedos < 4 ) {
      std::unique_ptr<Body2df> new_body = std::make_unique<Torpedo>(get_position(), get_angle(), get_velocity(), this);
      physics.add_body(new_body);
      shoot_cooldown.set_time(0.1);
      no_of_torpedos++;
      return true;
    }
  }
  return false;
}

bool Spaceship::contains_torpedo(Torpedo * torpedo) {
  return torpedo->get_origin() == this;
}

bool Spaceship::can_accelerate(float tick_time) {
  return (accelerate_timer <= 0.0 && ! is_marked_for_deletion() && ! in_hyperspace);
}

void Spaceship::accelerate(float  tick_time) {
  if (can_accelerate(tick_time) ) {
    accelerate_timer = 0.25f - tick_time;
    Body2df::accelerate(MAX_SPEED, std::min(0.25f, tick_time) );
  }
}


void Spaceship::deaccelerate(float tick_time) {
  if (! is_accelerating() && ! is_marked_for_deletion() && ! in_hyperspace) {
    // jede s ein 1/16 von der maximalen Geschwindigkeit abziehen
    const float speed = MAX_SPEED / 16.0f;
    float current_speed = velocity.length();
    if (current_speed > 0.0) {
      float deaccelerate_factor = tick_time * speed;
      set_velocity(velocity - deaccelerate_factor * (1.0f / current_speed) * velocity );
    }
  }
}

bool Spaceship::is_accelerating() {
  return ! is_marked_for_deletion() && ! in_hyperspace && accelerate_timer > 0.0f;
}

void Spaceship::turn_left(float tick_time) {
  if (! is_marked_for_deletion() && ! in_hyperspace) {
    turn(-PI / 0.6f, tick_time); // full turn takes 1.2 s
  }
}

void Spaceship::turn_right(float tick_time) {
  if (! is_marked_for_deletion() && ! in_hyperspace) {
    turn(PI / 0.6f, tick_time);
  }
}

void Spaceship::spaceship_fix(Body2df * body, float seconds) {
  displacement_fix(body, seconds);
  Spaceship * ship = static_cast<Spaceship *>(body);
  ship->pass_time(seconds);
}

void Spaceship::pass_time(float seconds) {
  if (hyperspace_delay > 0.0f && in_hyperspace) {
    hyperspace_delay -= seconds;
  }
  shoot_cooldown.tick(seconds);
  if (accelerate_timer > 0.0) {
    Body2df::accelerate(MAX_SPEED, seconds);
    accelerate_timer -= seconds;
  }
  if (turn_timer > 0.0) {
    turn_timer -= seconds;
  }
}

bool Spaceship::is_in_hyperspace() {
  return in_hyperspace;
}

void Spaceship::jump_into_hyperspace(Game & game) {
  if ( ! in_hyperspace && ! is_marked_for_deletion() ) {
    set_velocity({0.0f, 0.0f});
    set_position({512.0f + 348.0f * (0.5f - dis(gen)) , 368.0f + 256.0f * (0.5f - dis(gen)) });
    if ( dis(gen) < 0.25f ||  game.no_of_asteroids > (dis(gen) * 15.0f + 4.0f) ) {
      game.destroy_spaceship(); 
    } else {
      in_hyperspace = true;
      hyperspace_delay = HYPERSPACE_DELAY;
    }
  }
}

void Spaceship::jump_out_of_hyperspace(Game & game) {
  if ( in_hyperspace && hyperspace_delay <= 0.0 && ! is_marked_for_deletion() ) {
    BoundingVolume2df bounding{ get_position(), 50.0f };
    if  ( game.area_free_of_asteroids( &bounding ) ) {
      in_hyperspace = false;
    }
  }
}

void Spaceship::remove(Torpedo *torpedo) {
  if ( torpedo->get_origin() == this) {
    no_of_torpedos--;
    torpedo->set_origin(nullptr);
  }
}

bool Saucer::shoot(Game & game) {
  float direction_angle;
  if (shoot_cooldown.get_time() <= 0.0 && ! is_marked_for_deletion()) { 
    std::unique_ptr<Body2df> new_body;   
    if ( no_of_torpedos < 2) {
      if ( size == 0 && precise_shoot_counter <= 0 && game.ship_exists() ) {
        auto direct_shot = ( game.ship->get_position() - this->get_position() );
        direct_shot *= 1.0f /  direct_shot.length();        
        new_body = std::make_unique<Torpedo>(get_position(), direct_shot.angle(0.0f,1.0f), get_velocity(), this );
        precise_shoot_counter = 6;
      } else {
        direction_angle = PI * (1.0f - 2.0f * static_cast<float>(dis(gen)));
        new_body = std::make_unique<Torpedo>(get_position(), direction_angle, get_velocity(), this);
        precise_shoot_counter--;
      }
      no_of_torpedos++;
      game.physics.add_body(new_body);
      shoot_cooldown.set_time(0.75);
      return true;
    }
  }
  return false;
}



void Saucer::change_direction() {
  if ( change_direction_cooldown.get_time() < 0.0f && ! is_marked_for_deletion()) {
    float random = dis(gen);
    if ( random < 0.33 ) {
      velocity[1] = 0.0f;
    } else if (random < 0.66) {
      velocity[1] = 768.0f / 8.0f;
    } else {
      velocity[1] = -768.0f / 8.0f;
    }
    change_direction_cooldown.set_time(1.0f);
  }
}

void Saucer::pass_time(float seconds, Game & game) {
  if (shoot_cooldown.get_time() > 0.0) {
    shoot_cooldown.tick(seconds);
  } else if ( shoot(game) ) {
    game.game_events.push_back(GameEvent::torpedo_fired);
  }
  change_direction_cooldown.tick(seconds);
  if (change_direction_cooldown.get_time() < 0.0) {
    change_direction();
  }
}



void Game::saucer_fix(Body2df * body, float seconds) {
  Saucer * saucer = static_cast<Saucer *>(body);
  float x = saucer->get_position()[0];
  
  if ( x > SCREEN_WIDTH || x < 0.0f ) {
    remove(saucer);    
  } else {
    displacement_fix(body, seconds);
    saucer->pass_time(seconds, *this);
  }

}

short Saucer::get_size() const {
  return size;
}

void Saucer::remove(Torpedo *torpedo) {
  if ( this == torpedo->get_origin()) {
    no_of_torpedos--;
    torpedo->set_origin(nullptr);
  }
}


Game::Game() {
}

void Game::spawn_asteroids() {
  no_of_asteroids = current_no_of_asteroids;
  for (size_t i = 0; i < no_of_asteroids; i++) {
    vec2 position = {0, 0};
    float random = dis(gen);
    if ( random < 0.25 ) {
      position[0] = 128.0f * dis(gen);
      position[1] = 768.0f * dis(gen);
    } else if ( random < 0.5) {
      position[0] = 1024.0f - 128.0f * dis(gen);
      position[1] = 768.0f * dis(gen);
    } else if ( random < 0.75 ) {
      position[0] = 1024.0f * dis(gen);
      position[1] = 98.0f * dis(gen);
    } else {
      position[0] = 1024.0f * dis(gen);
      position[1] = 768.0f - 98.0f * dis(gen);      
    }
    std::unique_ptr<Body2df> new_body = std::make_unique<Asteroid>(3, position);    
    physics.add_body(new_body);
  }
  if (current_no_of_asteroids < MAXIMUM_ASTEROIDS_SPAWNING - 1) {
    current_no_of_asteroids += 2;
  } else if (current_no_of_asteroids == MAXIMUM_ASTEROIDS_SPAWNING - 1) {
    current_no_of_asteroids = MAXIMUM_ASTEROIDS_SPAWNING;
  }
  new_asteroids_spawn_timer = ASTEROID_SPAWN_TIME;
  saucer_timer = SAUCER_SPAWN_TIME;
  time_since_start_of_level = 0.0;
  game_events.push_back(GameEvent::next_level_started);
}

Physics2df & Game::get_physics() {
  return physics;
}


void Game::accelerate_ship(float tick_time) {
  if ( ship_exists() && ship->can_accelerate(tick_time) ) {
    game_events.push_back(GameEvent::ship_thrust);
    ship->accelerate(tick_time);
  }
}


void Game::destroy_asteroid(Asteroid * asteroid) {
  std::unique_ptr<Body2df> new_body = std::make_unique<Debris>(asteroid->get_position() );
  physics.add_body( new_body );
  switch (asteroid->get_size()) {
     case 1: game_events.push_back(GameEvent::small_asteroid_destroyed);
             break;
     case 2: game_events.push_back(GameEvent::medium_asteroid_destroyed);
             break;
     case 3: game_events.push_back(GameEvent::large_asteroid_destroyed);
             break;
  }
  if (asteroid->get_size() > 1) {
    if (no_of_asteroids < 26) {
      no_of_asteroids++;
      new_body = std::make_unique<Asteroid>(asteroid->get_size() - 1, asteroid->get_position() );
      physics.add_body( new_body );
    }
    asteroid->mark_for_deletion();
    new_body = std::make_unique<Asteroid>(asteroid->get_size() - 1, asteroid->get_position() );
    physics.add_body( new_body );
  } else {
    asteroid->mark_for_deletion();
    no_of_asteroids--;
  }
}

void Game::destroy_spaceship() {
  if ( ship_exists() ) {
    std::unique_ptr<Body2df> new_body = std::make_unique<SpaceshipDebris>(ship->get_position() );  
    physics.add_body( new_body );
    ship->mark_for_deletion();
    no_of_ships--;
    ship_spawn_timer = SHIP_SPAWN_TIME;
    ship = nullptr;
    game_events.push_back(GameEvent::ship_destroyed);
  }
}

void Game::asteroid_hits_spaceship(Asteroid * asteroid) {
  if ( ship_exists() && ! ship->is_in_hyperspace() ) {
    destroy_spaceship();
    destroy_asteroid(asteroid);
  }
}

void Game::add_score(long long points) {
  if ( (score + points ) / POINTS_EXTRA_SHIP > (score / POINTS_EXTRA_SHIP) ) {
    no_of_ships++;
    game_events.push_back(GameEvent::extra_ship_gained);
  }
  score += points;
}

void Game::torpedo_hits_asteroid(Torpedo * torpedo, Asteroid * asteroid) {
  destroy_asteroid(asteroid);
  torpedo->mark_for_deletion();
  if( ship == torpedo->get_origin() ) {
    switch ( asteroid->get_size() ) {
      case 1: add_score(POINTS_SMALL_ASTEROID);
              break;
      case 2: add_score(POINTS_MEDIUM_ASTEROID);
              break;
      case 3: add_score(POINTS_LARGE_ASTEROID);
              break;
    }
  }
}

void Game::destroy_saucer(Saucer * saucer) {
  switch (saucer->get_size()) {
     case 0: game_events.push_back(GameEvent::small_saucer_destroyed);
             break;
     case 1: game_events.push_back(GameEvent::big_saucer_destroyed);
             break;
  }
  std::unique_ptr<Body2df> new_body = std::make_unique<Debris>( saucer->get_position() );
  physics.add_body( new_body );
  remove(saucer);
}

void Game::spaceship_hits_saucer(Saucer * saucer) {
  destroy_spaceship();
  destroy_saucer(saucer);  
}

void Game::torpedo_hits_saucer(Torpedo * torpedo, Saucer * saucer) {
  torpedo->mark_for_deletion();
  if ( saucer->get_size() == 1 ) {
    add_score(POINTS_LARGE_SAUCER);
  } else {
    add_score(POINTS_SMALL_SAUCER);
  }
  destroy_saucer(saucer);
}

Spaceship * Game::get_ship() {
  return ship;
}

bool Game::area_free_of_asteroids(BoundingVolume2df * bounding) {
  return physics.is_area_free_of_bodies(bounding,
     [bounding](Body2df * body) -> bool { TypedBody * typed_body = static_cast<TypedBody *>(body);  
                                          return ! typed_body->is_marked_for_deletion()
                                                    && typed_body->get_type() == BodyType::asteroid;
                                        });                                
}

void Game::spawn_ship() {
  if ( saucer_exists() ) remove(saucer);
  BoundingVolume2df bounding{ vec2{512.0f, 368.0f}, 75.0f };
  if ( area_free_of_asteroids( &bounding ) ) {
    std::unique_ptr<Body2df> new_body = std::make_unique<Spaceship>(  vec2{512.0f, 368.0f} );
    ship = static_cast<Spaceship *>(new_body.get());
    physics.add_body(new_body);
  }
  game_events.push_back( GameEvent::new_ship_spawned );
}

void Game::hyperspace() {
  if (ship_exists() ) {
    ship->jump_into_hyperspace(*this);
  }
}

void Game::remove(Saucer * saucer) {
  saucer->mark_for_deletion();
  this->saucer = nullptr;
  saucer_timer = SAUCER_SPAWN_TIME;
}

void Game::tick(float tick_time) {
  debug(3, "tick() entry...");
  physics.tick(tick_time);  // collisions are handled during tick

  time_since_start_of_level += tick_time;
  saucer_timer -= tick_time;
  
  if ( ship_exists() && ship->is_in_hyperspace()) {
    ship->jump_out_of_hyperspace(*this);
  }

  if (ship_spawn_timer > 0) {
    ship_spawn_timer -= tick_time;
  }
  
  
  if (no_of_asteroids == 0 && ! saucer_exists() ) {
    if (new_asteroids_spawn_timer == 3.0f) {
      game_events.push_back(GameEvent::end_of_level);
    }
    if (new_asteroids_spawn_timer > 0.0f) {
      new_asteroids_spawn_timer -= tick_time;
    } else {
      spawn_asteroids();
    }
  }

  if (saucer_timer < 0.0 && ! saucer_exists() && no_of_asteroids > 0) {
    new_saucer();
  }
  if ( no_of_ships > 0 && ! ship_exists() && ship_spawn_timer <= 0.0 ) {
    spawn_ship();
  }
  
  if (ship_exists() ) {
    ship->deaccelerate(tick_time);
  }
  debug(3, "tick() exit.");
}

void Game::ship_shoots() {
  if ( ship_exists() && ship->shoot(physics) ) {
    game_events.push_back(GameEvent::torpedo_fired);
  }
}



void Game::new_saucer() {
  if ( ! saucer_exists() ) {
    short type = 1;
    if ( time_since_start_of_level > 35.0f || score >= 30000LL) {
      type = 0;
    }
    vec2 position = { 10.0,   dis(gen) * (SCREEN_HEIGHT / 10 + (6 * SCREEN_HEIGHT) / 8)  };
    vec2 velocity = { 1024.0f / 8.0f, 0.0 };
    BoundingVolume2df body{position, 10.0f};
    if ( area_free_of_asteroids(&body) ) {
      if ( dis(gen) > 0.5 ) {
        position[0] = SCREEN_WIDTH - 10.0;
        velocity[0] = -velocity[0];
      }
      std::unique_ptr<Body2df> new_body = std::make_unique<Saucer>( type, position, [&] (Body2df * body, float time)-> void { this->saucer_fix(body, time); });    
      saucer = static_cast<Saucer *>(new_body.get());
      saucer->set_velocity(velocity);
      physics.add_body( new_body );
      saucer_timer = 5.0;
    }
  }
}

bool Game::ship_exists() const {
  return ship != nullptr && ! ship->is_marked_for_deletion();
}

bool Game::saucer_exists() const {
  return saucer != nullptr && ! saucer->is_marked_for_deletion();
}

float Game::get_no_of_ships() const {
  return no_of_ships;
}

long long Game::get_score() const {
  return score;
}

float Game::get_time_since_start_of_level() const {
  return time_since_start_of_level;
}


std::vector<GameEvent> & Game::get_game_events() {
  return game_events;
}


void Game::resolve_collision(Body2df *body1, Body2df *body2) {
  TypedBody *typed_body1 = static_cast<TypedBody *>(body1);
  TypedBody *typed_body2 = static_cast<TypedBody *>(body2);
  Asteroid *asteroid;
  
  if (typed_body2->get_type() == BodyType::spaceship) {
    std::swap(typed_body1, typed_body2);
  }
  BodyType t1 = typed_body1->get_type();
  BodyType t2 = typed_body2->get_type();
  if (t1 == BodyType::spaceship) {
    if (t2 == BodyType::asteroid) {
      asteroid = static_cast<Asteroid *>(typed_body2);
      asteroid_hits_spaceship(asteroid);
    } else if (t2 == BodyType::saucer) {
      spaceship_hits_saucer(static_cast<Saucer *>(typed_body2));
    }
  }
  if (t2 == BodyType::torpedo) {
    std::swap(typed_body1, typed_body2);
    std::swap(t1, t2);
  } 
  
  if (t1 == BodyType::torpedo) {
    Torpedo * torpedo = static_cast<Torpedo *>(typed_body1);
    if (t2 == BodyType::asteroid) {
      asteroid = static_cast<Asteroid *>(typed_body2);
      torpedo_hits_asteroid(torpedo, asteroid);
    } else if (t2 == BodyType::spaceship) {
      if (! ship->is_in_hyperspace() ) {
        torpedo->mark_for_deletion();
        destroy_spaceship();
      }
    } else if (t2 == BodyType::saucer) {
      torpedo_hits_saucer(torpedo, static_cast<Saucer *>(typed_body2));
    }
  }
   
  if (t2 == BodyType::saucer) {
    std::swap(typed_body1, typed_body2);
    std::swap(t1, t2);
  }

  if (t1 == BodyType::saucer) {
    if (t2 == BodyType::asteroid) {
      destroy_saucer(static_cast<Saucer *>(typed_body1));
      destroy_asteroid(static_cast<Asteroid *>(typed_body2));
    }
  }    
}
  
bool Game::check_collision(Body2df *body1, Body2df *body2) {
  TypedBody *typed_body1 = static_cast<TypedBody *>(body1);
  TypedBody *typed_body2 = static_cast<TypedBody *>(body2);
  bool torpedo = typed_body1->get_type() == BodyType::torpedo
                  || typed_body2->get_type() == BodyType::torpedo;
  bool asteroid = typed_body1->get_type() == BodyType::asteroid
                  || typed_body2->get_type() == BodyType::asteroid;
  bool spaceship = typed_body1->get_type() == BodyType::spaceship
                  || typed_body2->get_type() == BodyType::spaceship;
  bool saucer = typed_body1->get_type() == BodyType::saucer
                  || typed_body2->get_type() == BodyType::saucer;
  return ( torpedo && (asteroid || spaceship || saucer ) )
       || (asteroid && (saucer || spaceship) )
       || (saucer && spaceship);  
}

void Game::resolve_deleted_bodies(Body2df *body1) {
  TypedBody *typed_body1 = static_cast<TypedBody *>(body1);
  if (typed_body1->get_type() == BodyType::torpedo) {
    Torpedo * torpedo = static_cast<Torpedo *>(typed_body1);
    TypedBody * origin = torpedo->get_origin();
    if (origin->get_type() == BodyType::saucer) {
      Saucer * saucer = static_cast<Saucer *>(origin);
      saucer->remove(torpedo);
    } else if (origin->get_type() == BodyType::spaceship) {
      ship->remove(torpedo);
    }
  }
}

  
