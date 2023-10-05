#ifndef BODY_H
#define BODY_H

#include <cmath>
#include <vector>
#include <functional>
#include <iostream>

#include "math.h"
#include "timer.h"
#include "geometry.h"


// a bounding "box" based on a sphere
template<class FLOAT_TYPE, size_t N>
class BoundingVolumeCircle : public Sphere<FLOAT_TYPE, N> {
public:
  BoundingVolumeCircle(Vector<FLOAT_TYPE,N> position, FLOAT_TYPE radius);

  bool collides(BoundingVolumeCircle<FLOAT_TYPE, N> volume) const;

  FLOAT_TYPE get_radius() const;
  
  Vector<FLOAT_TYPE,N> get_position() const;
    
  void set_position(Vector<FLOAT_TYPE,N> position);  
  
};


// a (N-dimensional) axis aligned bounding box
template<class FLOAT_TYPE, size_t N>
class BoundingVolumeHyperRectangle {
protected:
  Vector<FLOAT_TYPE, N> position;
  Vector<FLOAT_TYPE, N> edge_lengths;
public:
  BoundingVolumeHyperRectangle(Vector<FLOAT_TYPE,N> position, Vector<FLOAT_TYPE,N> edge_lengths );

  bool collides(BoundingVolumeHyperRectangle<FLOAT_TYPE, N> volume) const;

  FLOAT_TYPE get_edge_length(size_t edge) const;
  
  Vector<FLOAT_TYPE,N> get_position() const;
    
  void set_position(Vector<FLOAT_TYPE,N> position);  
  
};

template<class FLOAT_TYPE, size_t N, class BV> class Physics;

// dynamic physical body  with a bounding value of type BV
// the body has a (central) position, a velocity, an orientation defined by an angle and other physical attributes
template<class FLOAT_TYPE, size_t N, class BV>
class Body  {
protected:
  BV bounding;
  Vector<FLOAT_TYPE, N> velocity;
  FLOAT_TYPE max_velocity;
  FLOAT_TYPE min_velocity;
  FLOAT_TYPE angle;

  std::function<void(Body<FLOAT_TYPE, N, BV> *, FLOAT_TYPE)> fix; // fix object values after movement

  Counter delete_counter;
  bool deletable = false;
public:
  Body(  BV bounding_volume,
         Vector<FLOAT_TYPE, N> velocity, 
         FLOAT_TYPE max_velocity = 1.0,
         FLOAT_TYPE min_velocity = 0.0,
         FLOAT_TYPE angle = 0.0,

         std::function<void(Body<FLOAT_TYPE, N, BV> *, FLOAT_TYPE)> fix 

            = [](Body<FLOAT_TYPE, N, BV> * , FLOAT_TYPE ) -> void {  }); 

 

 void move(FLOAT_TYPE seconds = 1.0);

  

  // turns the Body in the x/y-Plane 
  // angle is measured in radians
  void turn(FLOAT_TYPE angle, FLOAT_TYPE seconds = 1.0);

  void set_velocity(Vector<FLOAT_TYPE, N> velocity);
  
  void accelerate(FLOAT_TYPE acceleration, FLOAT_TYPE seconds = 1.0);
  
  void bounce(size_t coordinate);
  
  Vector<FLOAT_TYPE, N> get_velocity() const;
  
  void mark_for_deletion();

  // removes this Body from the physics when physics.tick() is called the next time  
  bool is_marked_for_deletion() const;
  
  FLOAT_TYPE get_angle() const;
  
  void set_time_to_delete(FLOAT_TYPE time_to_delete);
  
  FLOAT_TYPE get_time_to_delete() const;

  Vector<FLOAT_TYPE,N> get_position() const;
    
  void set_position(Vector<FLOAT_TYPE,N> position);  
  
  friend class Physics<FLOAT_TYPE, N, BV>;

  BV get_bounding_volume() const;
};


// a basic physic engine controlling the movements and collisions of Body-objects
// the collisions are resolved with callback handlers
template<class FLOAT_TYPE, size_t N, class BV>
class Physics {
  // all Body objects controlled by the engine
  std::vector<Body<FLOAT_TYPE, N, BV> *> bodies;

  // Body objects waiting to be added to this engine in the next call to tick()
  std::vector<Body<FLOAT_TYPE, N, BV> *> bodies_to_add;

  // collision callback that returns true if the collision of to Body objects has to be resolved
  std::function<bool(Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *)> check_collision;
  
  // callback that is responsible for resolving the collision
  std::function<void(Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *)> resolve_collision;
  FLOAT_TYPE tick_time = 1.0;
public:

  Physics( std::function<bool(Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *)> check_collision
                   = [](Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> * ) -> bool { return true; },
                   
           std::function<void(Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *)> resolve_collision 
                              = [](Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *) -> void { }
         );

  void set_tick_time(FLOAT_TYPE tick_time);

  // returns the tick_time which was used during the last tick 
  FLOAT_TYPE get_tick_time();

  // adds a new Body object to this engine
  // the body is added in the next call to tick()  
  void add_body(Body<FLOAT_TYPE, N, BV> * body);
  
  Body<FLOAT_TYPE, N, BV> * get_body(size_t i);
  
  const std::vector<Body<FLOAT_TYPE, N, BV> *> & get_bodies();

  // Peforms the follown steps in the given order:
  // 1. adds all new Body object to this engine,
  // 2. removes all Body object, that has to be deleted from it
  // 3. moves all objects according to the current tick_time
  // 4. checks for collisions and uses the callback handler to resolve them
  // 5. removes all Body objects, that has to be deleted
  void tick();
  
  void tick(FLOAT_TYPE tick_time);
  
  bool is_area_free_of_bodies(BV * area,
                              std::function<bool(Body<FLOAT_TYPE, N, BV> *)> check_body
                                = [](Body<FLOAT_TYPE, N, BV> * body) -> bool {return ! body->is_marked_for_deletion();});
};


typedef BoundingVolumeCircle<float, 2u> BoundingVolume2df;
typedef Body<float, 2u, BoundingVolume2df> Body2df;
typedef Physics<float, 2u, BoundingVolume2df> Physics2df;

typedef BoundingVolumeHyperRectangle<float, 2u> Rectangle2df;
typedef Body<float, 2u, Rectangle2df> BodyRect2df;
typedef Physics<float, 2u, Rectangle2df> PhysicsRect2df;

#endif
