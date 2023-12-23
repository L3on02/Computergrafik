#ifndef GAME_H
#define GAME_H

#include <vector>  
#include <utility>
#include <array>
#include <random>
#include <memory>
#include "timer.h"
#include "physics.h" 

// all different types of object used in this Asteroid-Game
// for each type there will be a corresponding class
enum class BodyType : short { spaceship, asteroid, torpedo, saucer, spaceship_debris, debris };

// these games events are generated during each tick and can, for instance, be used to
// generate special view or sound effects
enum class GameEvent : short { small_asteroid_destroyed, medium_asteroid_destroyed, large_asteroid_destroyed,
                                extra_ship_gained, ship_destroyed, ship_thrust,
                                small_saucer_destroyed, big_saucer_destroyed, end_of_level, 
                                next_level_started, new_ship_spawned, torpedo_fired };


class Game;

void displacement_fix(Body2df * body, float seconds = 1.0);

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> dis(0.0, 0.99);

// the base class of all game objects
class TypedBody : public Body2df {
protected:
  BodyType type;
public:
  TypedBody(BodyType type, Body2df body) : Body2df(body), type(type) { }

  BodyType get_type() {
    return type;
  }
};

class Asteroid : public TypedBody {
short size; // 3 = big, 2 = medium, 1 = small
short rock_type; // one of the four different rock types
public:
  Asteroid(short size = 3);

  Asteroid(short size, vec2 position );
  
  short get_size() const;
  
  short get_rock_type() const;
};

class Torpedo : public TypedBody {
static constexpr float MAX_SPEED = 768.0f;
TypedBody * origin;
public:

  Torpedo()
    : Torpedo( vec2{0.0f, 0.0f}, 0.0f, vec2{1.0f, 1.0f}, nullptr) 
    { 
      this->origin = origin; 
    }


  Torpedo(vec2 position, float angle, vec2 velocity, TypedBody * origin)
    : TypedBody(BodyType::torpedo, 
                Body2df{ BoundingVolume2df{position + 14.0f * vec2( angle ), 1.0},
                         velocity + 1.1f * MAX_SPEED / 2.0f * vec2( angle ),
                         MAX_SPEED, 0.0f, angle, displacement_fix} ) 
    { set_time_to_delete(1.2f);
      this->origin = origin; 
    }

  TypedBody * get_origin() {
    return origin;
  }
  
  void set_origin(TypedBody * origin) {
    this->origin = origin;
  }
};

class Spaceship : public TypedBody {
  static constexpr float HYPERSPACE_DELAY = 1.0f;
  Counter shoot_cooldown;
  float accelerate_timer = 0.0f;
  float turn_timer = 0.0f;
  float hyperspace_delay = 0.0f;
  bool in_hyperspace = false;
  size_t no_of_torpedos = 0;
public:
  static constexpr float MAX_SPEED = 384.0f;
  static void spaceship_fix(Body2df * body, float seconds);
  Spaceship(vec2 position)
    : TypedBody(BodyType::spaceship,
                Body2df{ BoundingVolume2df{position, 10.0f},
                         vec2{0.0f, 0.0f}, MAX_SPEED, 0.0f, 0.0f, spaceship_fix} )
    {
    }
  bool contains_torpedo(Torpedo * torpedo);
  bool shoot(Physics2df & physics);
  bool is_in_hyperspace();
  void pass_time(float seconds);
  bool can_accelerate(float seconds);
  void accelerate(float seconds);
  void deaccelerate(float seconds);
  bool is_accelerating();
  void turn_left(float seconds);
  void turn_right(float seconds);
  void jump_into_hyperspace(Game & game);
  void jump_out_of_hyperspace(Game & game);
  void remove(Torpedo *torpedo);
};



class SpaceshipDebris : public TypedBody {
public:
  static constexpr float TIME_TO_DELETE = 3.0;
  SpaceshipDebris(vec2 position = vec2{0.0, 0.0}, float angle = 0.0)
    : TypedBody(BodyType::spaceship_debris,
                Body2df{ BoundingVolume2df{position, 0.0},
                         vec2{0.0, 0.0}, 384.0, 0.0, angle, displacement_fix} )
  {
    set_time_to_delete(TIME_TO_DELETE);
  }
};


class Saucer : public TypedBody {
  Counter shoot_cooldown{1.0f};
  Counter change_direction_cooldown{4.0f};
  short size; // 0 = small, 1 = big
  char precise_shoot_counter = 0; // every sixth torpedo of a small saucer shoots in direction to the spaceship
  size_t no_of_torpedos = 0;
public:
  Saucer(short size = 1, vec2 position = vec2{0.0, 0.0}, std::function<void(Body2df *, float)> saucer_fix = displacement_fix)
    : TypedBody(BodyType::saucer,
                Body2df{ BoundingVolume2df{position, static_cast<float>(size == 1 ? 15 : 7) },
                         vec2{0.0, 0.0}, 200.0, 0.0, 0.0, saucer_fix} ) 
    {
      this->size = size;
      if (size == 0) {
        shoot_cooldown.set_time(0.6);
      }
    }
  bool shoot(Game & game);
  void change_direction();
  void pass_time(float seconds, Game & game);
  short get_size() const;
  void remove(Torpedo *torpedo);
};


// asteroid or saucer debris
class Debris : public TypedBody {
public:
  static constexpr float TIME_TO_DELETE = 0.6f;
  Debris(vec2 position = vec2{0.0, 0.0}, float angle = 0.0f)
    : TypedBody( BodyType::debris,
                 Body2df{ BoundingVolume2df{position, 0.0f},
                          vec2{0.0, 0.0}, 0.0f, 0.0f, angle, displacement_fix })
  {
    set_time_to_delete(TIME_TO_DELETE);
  }
};


// Game is a facade storing and giving access to all game objects
class Game {
  static constexpr int POINTS_SMALL_SAUCER = 1000;
  static constexpr int POINTS_LARGE_SAUCER = 200;
  static constexpr int POINTS_SMALL_ASTEROID = 100;
  static constexpr int POINTS_MEDIUM_ASTEROID = 50;
  static constexpr int POINTS_LARGE_ASTEROID = 20;
  static constexpr int POINTS_EXTRA_SHIP = 10000;

  static constexpr float SAUCER_SPAWN_TIME = 12.0f;
  static constexpr float SHIP_SPAWN_TIME = 4.0f;
  static constexpr float ASTEROID_SPAWN_TIME = 3.0f;

  static constexpr short NO_OF_SHIPS_AT_START = 3;
  static constexpr short NO_OF_ASTEROIDS_AT_START = 4;
  static constexpr short MAXIMUM_ASTEROIDS_SPAWNING = 11;
  void saucer_fix(Body2df * body, float seconds);
  Physics2df physics{ [&](Body2df * b1, Body2df * b2) -> bool { return this->check_collision(b1, b2); },
                      [&](Body2df * b1, Body2df * b2) -> void { this->resolve_collision(b1, b2); },
                      [&](Body2df * b1) -> void { this->resolve_deleted_bodies(b1); }
                    };
  Spaceship * ship = nullptr;
  Saucer * saucer = nullptr;
  std::vector<GameEvent> game_events;
  short no_of_ships = NO_OF_SHIPS_AT_START;
  size_t current_no_of_asteroids = NO_OF_ASTEROIDS_AT_START; // no of asteroids at start of current level
  size_t no_of_asteroids = 0;
  long long score = 0LL;
  // check_collision must not have side effects
  bool check_collision(Body2df *body1, Body2df *body2);
  void resolve_collision(Body2df *body1, Body2df *body2);
  void resolve_deleted_bodies(Body2df *body1);
  void destroy_asteroid(Asteroid * asteroid);
  void spawn_ship();
  void spawn_asteroids();
  void destroy_spaceship();
  void destroy_saucer(Saucer *saucer);
  void asteroid_hits_spaceship(Asteroid * asteroid);
  void torpedo_hits_asteroid(Torpedo * torpedo, Asteroid * asteroid);
  void spaceship_hits_saucer(Saucer * saucer);
  void torpedo_hits_saucer(Torpedo * torpedo, Saucer * saucer);
  float time_since_start_of_level = 0.0;
  float saucer_timer = SHIP_SPAWN_TIME;
  float ship_spawn_timer = 0.0;
  float new_asteroids_spawn_timer = 0.0;
  void new_saucer();
  void add_score(long long points);
  bool area_free_of_asteroids(BoundingVolume2df * bounding);
  void remove(Saucer * saucer);
public:
  Game();
  void tick(float tick_time);
  void ship_shoots();
  void hyperspace();
  void accelerate_ship(float tick_time);
  float get_no_of_ships() const;
  long long get_score() const;
  float get_time_since_start_of_level() const;
  bool ship_exists() const;
  bool saucer_exists() const;
  Spaceship * get_ship();
  Physics2df & get_physics();
  std::vector<GameEvent> & get_game_events();  
  friend class Saucer;
  friend class Spaceship;
};



#endif
