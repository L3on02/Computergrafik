#include <utility>

template<class FLOAT_TYPE, size_t N>
BoundingVolumeCircle<FLOAT_TYPE, N>::BoundingVolumeCircle(Vector<FLOAT_TYPE,N> position, FLOAT_TYPE radius) 
 : Sphere<FLOAT_TYPE, N>(position, radius) { }

template<class FLOAT_TYPE, size_t N>
bool BoundingVolumeCircle<FLOAT_TYPE, N>::collides(BoundingVolumeCircle<FLOAT_TYPE, N> volume) const {
  return this->intersects(volume);
}

template<class FLOAT_TYPE, size_t N>  
FLOAT_TYPE BoundingVolumeCircle<FLOAT_TYPE, N>::get_radius() const {
  return this->radius;
}

template<class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE,N> BoundingVolumeCircle<FLOAT_TYPE, N>::get_position() const {
  return this->center;
}


template<class FLOAT_TYPE, size_t N>  
void BoundingVolumeCircle<FLOAT_TYPE, N>::set_position(Vector<FLOAT_TYPE,N> position) {
  this->center = position;
}

template<class FLOAT_TYPE, size_t N>  
BoundingVolumeHyperRectangle<FLOAT_TYPE,N>::BoundingVolumeHyperRectangle(Vector<FLOAT_TYPE,N> position, Vector<FLOAT_TYPE,N> edge_lengths )
 : position(position), edge_lengths(edge_lengths) { }

template<class FLOAT_TYPE, size_t N>  
bool BoundingVolumeHyperRectangle<FLOAT_TYPE,N>::collides(BoundingVolumeHyperRectangle<FLOAT_TYPE, N> volume) const {
 bool collision = true;
 for (size_t axis = 0u; axis < N; axis++) {
   collision &= position[axis] <= volume.edge_lengths[axis] + volume.position[axis];
   collision &= position[axis] + edge_lengths[axis] >= volume.position[axis];
 }
 return collision;
}

template<class FLOAT_TYPE, size_t N>  
FLOAT_TYPE BoundingVolumeHyperRectangle<FLOAT_TYPE,N>::get_edge_length(size_t edge) const {
  return edge_lengths[edge];
}
  
template<class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE,N> BoundingVolumeHyperRectangle<FLOAT_TYPE,N>::get_position() const {
  return position;
}
    
template<class FLOAT_TYPE, size_t N>  
void BoundingVolumeHyperRectangle<FLOAT_TYPE,N>::set_position(Vector<FLOAT_TYPE,N> position) {
  this->position = position;
}


template<class FLOAT_TYPE, size_t N, class BV> class Physics;

template<class FLOAT_TYPE, size_t N, class BV>
Body<FLOAT_TYPE, N, BV>::Body(
       BV bounding_volume,
       Vector<FLOAT_TYPE, N> velocity,
       FLOAT_TYPE max_velocity,
       FLOAT_TYPE min_velocity,
       FLOAT_TYPE angle,
       std::function<void(Body<FLOAT_TYPE, N, BV> *, FLOAT_TYPE)> fix)
  : bounding(bounding_volume), velocity(velocity), max_velocity(max_velocity),
    min_velocity(min_velocity), angle(angle)
    {
      this->fix = fix;
      delete_counter.set_time(0.0);
    }
 
template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::move(FLOAT_TYPE seconds) {
  set_position( get_position() +  seconds * velocity);
  delete_counter.tick(seconds);
  fix(this, seconds);
}
  
// turns the Body in the x/y-Plane 
// angle is measured in radians
template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::turn(FLOAT_TYPE angle, FLOAT_TYPE seconds) {
  this->angle += seconds * angle;
}

template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::set_velocity(Vector<FLOAT_TYPE, N> velocity) {
  if (velocity.length() > max_velocity) {
    velocity = (1.0f / velocity.length()) * max_velocity  * velocity;
  }
  if (velocity.length() < min_velocity) {
    velocity = (min_velocity / velocity.length()) * velocity;
  }
  this->velocity = velocity;
}
  
template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::accelerate(FLOAT_TYPE acceleration, FLOAT_TYPE seconds) {
  if (N >= 2) {
    Vector<FLOAT_TYPE, N> velocity = this->velocity + seconds * acceleration * Vector<FLOAT_TYPE,N>{ std::cos(angle), std::sin(angle) };   
    set_velocity(velocity);
  }
}

template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::bounce(size_t coordinate) {
  velocity[coordinate] = -velocity[coordinate];
}

template<class FLOAT_TYPE, size_t N, class BV>
Vector<FLOAT_TYPE, N> Body<FLOAT_TYPE, N, BV>::get_velocity() const {
  return velocity;
}
template<class FLOAT_TYPE, size_t N, class BV>
Vector<FLOAT_TYPE, N> Body<FLOAT_TYPE, N, BV>::get_position() const {
  return bounding.get_position();
}
    
template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::set_position(Vector<FLOAT_TYPE,N> position) {
  bounding.set_position(position);
}


template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::mark_for_deletion() {
  set_time_to_delete(0.0);
}


template<class FLOAT_TYPE, size_t N, class BV>
bool Body<FLOAT_TYPE, N, BV>::is_marked_for_deletion() const {
  return deletable && delete_counter.get_time() <= 0.0;
}

template<class FLOAT_TYPE, size_t N, class BV>
FLOAT_TYPE Body<FLOAT_TYPE, N, BV>::get_angle() const {
  return angle;
}

template<class FLOAT_TYPE, size_t N, class BV>
void Body<FLOAT_TYPE, N, BV>::set_time_to_delete(FLOAT_TYPE time_to_delete) {
  time_to_delete = std::max(time_to_delete, static_cast<FLOAT_TYPE>(0.0));
  this->deletable = true;
  delete_counter.set_time(time_to_delete);
}

template<class FLOAT_TYPE, size_t N, class BV>
FLOAT_TYPE Body<FLOAT_TYPE, N, BV>::get_time_to_delete() const {
  return delete_counter.get_time();
}

template<class FLOAT_TYPE, size_t N, class BV>

BV Body<FLOAT_TYPE, N, BV>::get_bounding_volume() const {
  return bounding;
}




template<class FLOAT_TYPE, size_t N, class BV>
Physics<FLOAT_TYPE, N, BV>::Physics( std::function<bool(Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *)> check_collision,
                                 std::function<void(Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *)> resolve_collision)
  : check_collision(check_collision), resolve_collision(resolve_collision) { }
         
  
template<class FLOAT_TYPE, size_t N, class BV>
void Physics<FLOAT_TYPE, N, BV>::set_tick_time(FLOAT_TYPE tick_time) {
  this->tick_time = tick_time;
}   

template<class FLOAT_TYPE, size_t N, class BV>
FLOAT_TYPE Physics<FLOAT_TYPE, N, BV>::get_tick_time() {
  return tick_time;
}   
  
template<class FLOAT_TYPE, size_t N, class BV>
void Physics<FLOAT_TYPE, N, BV>::add_body(Body<FLOAT_TYPE, N, BV> * body) {
  if ( std::find( bodies.begin(), bodies.end(), body) == bodies.end() ) {
    bodies_to_add.push_back(body);
  } else {
    std::cout << "body alread in physics" << std::endl;
  }
}


template<class FLOAT_TYPE, size_t N, class BV>
Body<FLOAT_TYPE, N, BV> * Physics<FLOAT_TYPE, N, BV>::get_body(size_t i) {
  return bodies[i];
}

template<class FLOAT_TYPE, size_t N, class BV>
const std::vector<Body<FLOAT_TYPE, N, BV> *> & Physics<FLOAT_TYPE, N, BV>::get_bodies() {
  return bodies;
}  

template<class FLOAT_TYPE, size_t N, class BV>
bool Physics<FLOAT_TYPE, N, BV>::is_area_free_of_bodies(BV * area, std::function<bool(Body<FLOAT_TYPE, N, BV> *)> check_body) {
  for (Body<FLOAT_TYPE, N, BV> * body : bodies) {
    if ( check_body(body) && body->bounding.collides(*area) ) {
      return false;
    }
  }
  return true;
}

template<class FLOAT_TYPE, size_t N, class BV>
void Physics<FLOAT_TYPE, N, BV>::tick() {
  Physics<FLOAT_TYPE, N, BV>::tick(tick_time);
}

template<class FLOAT_TYPE, size_t N, class BV>
void Physics<FLOAT_TYPE, N, BV>::tick(FLOAT_TYPE tick_time) {
  set_tick_time(tick_time);
  std::vector< std::pair<Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *> > bodies_to_resolve;
  bodies.insert(bodies.end(), bodies_to_add.begin(), bodies_to_add.end() ); 
  bodies_to_add.clear();    

  erase_if(bodies, [](Body<FLOAT_TYPE, N, BV> * body) { return body->is_marked_for_deletion();}); 

  for (auto body : bodies) {
    body->move(tick_time);
  }
   
  for (auto iterator1 = bodies.begin(); iterator1 != bodies.end(); iterator1++ ) {
    for (auto iterator2 = iterator1 + 1; iterator2 != bodies.end(); iterator2++) {
      if ( (*iterator1)->bounding.collides( (*iterator2)->bounding)   ) {
        if (check_collision(*iterator1, *iterator2) ) {
          bodies_to_resolve.push_back( std::pair<Body<FLOAT_TYPE, N, BV> *, Body<FLOAT_TYPE, N, BV> *>(*iterator1, *iterator2) );
        }
      }
    }
  }

  for (auto pair : bodies_to_resolve) {
    resolve_collision(pair.first, pair.second);
  }    
  erase_if(bodies, [](Body<FLOAT_TYPE, N, BV> * body) { return body->is_marked_for_deletion();}); 
}

