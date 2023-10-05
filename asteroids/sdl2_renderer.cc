#include "sdl2_renderer.h"
#include <span>
#include <utility>


void SDL2Renderer::renderSpaceship(Vector2df position, float angle) {
    static std::array<SDL_Point, 6> ship_points{SDL_Point{-6, 3},
                                              SDL_Point{-6,-3},
                                              SDL_Point{-10,-6},
                                              SDL_Point{ 14, 0},
                                              SDL_Point{-10, 6},
                                              SDL_Point{-6, 3}};
  
  std::array<SDL_Point, ship_points.size()> points;

  float cos_angle = std::cos(angle);
  float sin_angle = std::sin(angle);
  for (size_t i = 0; i < ship_points.size(); i++) {
    float x = ship_points[i].x;
    float y = ship_points[i].y;
    points[i].x = (cos_angle * x - sin_angle * y) + position[0];
    points[i].y = (sin_angle * x + cos_angle * y) + position[1];
  }
  SDL_RenderDrawLines(renderer, points.data(), points.size());

}

void SDL2Renderer::render(Spaceship * ship) {
  static SDL_Point flame_points[] { {-6, 3}, {-12, 0}, {-6, -3} };
  std::array<SDL_Point, std::span{flame_points}.size()> points;

  if (! ship->is_in_hyperspace()) {
    if (ship->is_accelerating()) {
      float cos_angle = std::cos(ship->get_angle());
      float sin_angle = std::sin(ship->get_angle());
      for (size_t i = 0; i < points.size(); i++) {
        float x = flame_points[i].x;
        float y = flame_points[i].y;
        points[i].x = (cos_angle * x - sin_angle * y) + ship->get_position()[0];
        points[i].y = (sin_angle * x + cos_angle * y) + ship->get_position()[1];
      }
       SDL_RenderDrawLines(renderer, points.data(), points.size());
    }
  renderSpaceship(ship->get_position(), ship->get_angle());  
  }
}

void SDL2Renderer::render(Saucer * saucer) {
  static SDL_Point saucer_points[] = { {-16, -6}, {16, -6}, {40, 6}, {-40, 6}, {-16, 18}, {16, 18},
                                       {40, 6}, {16, -6}, {8, -18}, {-8, -18}, {-16, -6}, {-40, 6} };
  
  std::array<SDL_Point, std::span{saucer_points}.size()> points;

  Vector2df position = saucer->get_position();
  float scale = 0.5;
  if ( saucer->get_size() == 0 ) {
    scale = 0.25;
  }
  for (size_t i = 0; i < points.size(); i++) {
    float x = saucer_points[i].x;
    float y = saucer_points[i].y;
    points[i].x = scale * x + position[0];
    points[i].y = scale * y + position[1];
  }
  SDL_RenderDrawLines(renderer, points.data(), points.size());
}


void SDL2Renderer::render(Torpedo * torpedo) {
  SDL_RenderDrawPoint(renderer, torpedo->get_position()[0], torpedo->get_position()[1]);
  SDL_RenderDrawPoint(renderer, torpedo->get_position()[0] + 1, torpedo->get_position()[1]);
  SDL_RenderDrawPoint(renderer, torpedo->get_position()[0], torpedo->get_position()[1] - 1);
  SDL_RenderDrawPoint(renderer, torpedo->get_position()[0], torpedo->get_position()[1] + 1);
  SDL_RenderDrawPoint(renderer, torpedo->get_position()[0] - 1, torpedo->get_position()[1]);
}
  
void SDL2Renderer::render(Asteroid * asteroid) {
  static SDL_Point asteroids_points1[] = {
    { 0, -12}, {16, -24}, {32, -12}, {24, 0}, {32, 12}, {8, 24}, {-16, 24}, {-32, 12}, {-32, -12}, {-16, -24}, {0, -12}
  };   
  static SDL_Point asteroids_points2[] = {
    { 16, -6}, {32, -12}, {16, -24}, {0, -16}, {-16, -24}, {-24, -12}, {-16, -0}, {-32, 12}, {-16, 24}, {-8, 16}, {16, 24}, {32, 6}, {16, -6}
  }; 
  static SDL_Point asteroids_points3[] = {
    {-16, 0}, {-32, 6}, {-16, 24}, {0, 6}, {0, 24}, {16, 24}, {32, 6}, {32, 6}, {16, -24}, {-8, -24}, {-32, -6}, {-16, 0}
  };
  static SDL_Point asteroids_points4[] = {  
    {8,0}, {32,-6}, {32, -12}, {8, -24}, {-16, -24}, {-8, -12}, {-32, -12}, {-32, 12}, {-16, 24}, {8, 16}, {16, 24}, {32, 12}, {8, 0}
  };
  static size_t sizes[] = {std::span{asteroids_points1}.size(),
                           std::span{asteroids_points2}.size(),
                           std::span{asteroids_points3}.size(),
                           std::span{asteroids_points4}.size() };
  size_t size = sizes[ asteroid->get_rock_type() ];
  SDL_Point * asteroids_points = asteroids_points1;
  if ( asteroid->get_rock_type() == 1 ) asteroids_points = asteroids_points2;
  if ( asteroid->get_rock_type() == 2 ) asteroids_points = asteroids_points3;
  if ( asteroid->get_rock_type() == 3 ) asteroids_points = asteroids_points4;
 
  SDL_Point points[std::span{asteroids_points4}.size()];
  
  float scale = (asteroid->get_size() == 3 ? 1.0 : ( asteroid->get_size() == 2 ? 0.5 : 0.25 ));
  Vector2df position = asteroid->get_position();
  for (size_t i = 0; i < size; i++) {
    points[i].x = scale * asteroids_points[i].x + position[0];
    points[i].y = scale * asteroids_points[i].y + position[1];
  }
  SDL_RenderDrawLines(renderer, points, size);
}


void SDL2Renderer::render(SpaceshipDebris * debris) {
  static SDL_Point ship_points[6][2] = {{ SDL_Point{-2, -1}, SDL_Point{-10, 7} }, 
                                        { SDL_Point{3, 1}, SDL_Point{7, 8} },
                                        { SDL_Point{0, 3}, SDL_Point{6, 1} },
                                        { SDL_Point{3, -1}, SDL_Point{ -5, -7} },
                                        { SDL_Point{0, -4}, SDL_Point{-6, -6} },
                                        { SDL_Point{-2, 2}, SDL_Point{2, 5} } };
  static std::array<Vector2df, 6> debris_direction = { Vector2df{-40, -23}, Vector2df{50, 15}, Vector2df{0, 45},
                                                       Vector2df{60, -15}, Vector2df{10, -52}, Vector2df{-40, 30} };
  Vector2df position = debris->get_position();
  std::array<SDL_Point, 4> points;
  float scale =  0.2 * (SpaceshipDebris::TIME_TO_DELETE - debris->get_time_to_delete());
  for (size_t i = 0; i < debris_direction.size(); i++) {
    points[0].x = scale * debris_direction[i][0] + ship_points[i][0].x + position[0];
    points[0].y = scale * debris_direction[i][1] + ship_points[i][0].y + position[1];
    points[1].x = scale * debris_direction[i][0] + ship_points[i][1].x + position[0];
    points[1].y = scale * debris_direction[i][1] + ship_points[i][1].y + position[1];
    if ( debris->get_time_to_delete() >= 0.5 * i )  {
      SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[1].x, points[1].y );    
    }
  }
                                  
}

void SDL2Renderer::render(Debris * debris) {
  static SDL_Point debris_points[] = { {-32, 32}, {-32, -16}, {-16, 0}, {-16, -32}, {-8, 24}, {8, -24}, {24, 32}, {24, -24}, {24, -32}, {32, -8} };

  static SDL_Point point;
  Vector2df position = debris->get_position();
  for (size_t i = 0; i < std::span{debris_points}.size(); i++) {
    point.x = (Debris::TIME_TO_DELETE - debris->get_time_to_delete()) * debris_points[i].x + position[0];
    point.y = (Debris::TIME_TO_DELETE - debris->get_time_to_delete()) * debris_points[i].y + position[1];
    SDL_RenderDrawPoint(renderer, point.x, point.y);
  }
}

void SDL2Renderer::renderFreeShips() {
  constexpr float FREE_SHIP_X = 128;
  constexpr float FREE_SHIP_Y = 64;
  Vector2df position = {FREE_SHIP_X, FREE_SHIP_Y};
  
  for (int i = 0; i < game.get_no_of_ships(); i++) {
    renderSpaceship( position, -PI / 2.0 );
    position[0] += 20.0;
  }
}




void SDL2Renderer::renderScore() {
  constexpr float SCORE_X = 128 - 48;
  constexpr float SCORE_Y = 48 - 4;
  
  static SDL_Point digit_0[] = { {0,-8}, {4,-8}, {4,0}, {0,0}, {0, -8} };
  static SDL_Point digit_1[] = { {4,0}, {4,-8} };
  static SDL_Point digit_2[] = { {0,-8}, {4,-8}, {4,-4}, {0,-4}, {0,0}, {4,0}  };
  static SDL_Point digit_3[] = { {0,0}, {4, 0}, {4,-4}, {0,-4}, {4,-4}, {4, -8}, {0, -8}  };
  static SDL_Point digit_4[] = { {4,0}, {4,-8}, {4,-4}, {0,-4}, {0,-8}  };
  static SDL_Point digit_5[] = { {0,0}, {4,0}, {4,-4}, {0,-4}, {0,-8}, {4, -8}  };
  static SDL_Point digit_6[] = { {0,-8}, {0,0}, {4,0}, {4,-4}, {0,-4} };
  static SDL_Point digit_7[] = { {0,-8}, {4,-8}, {4,0} };
  static SDL_Point digit_8[] = { {0,-8}, {4,-8}, {4,0}, {0,0}, {0,-8}, {0, -4}, {4, -4} };
  static SDL_Point digit_9[] = { {4, 0}, {4,-8}, {0,-8}, {0, -4}, {4, -4} };
  
  static size_t sizes[] = { std::span{digit_0}.size(), 
                            std::span{digit_1}.size(), 
                            std::span{digit_2}.size(), 
                            std::span{digit_3}.size(), 
                            std::span{digit_4}.size(), 
                            std::span{digit_5}.size(), 
                            std::span{digit_6}.size(), 
                            std::span{digit_7}.size(), 
                            std::span{digit_8}.size(), 
                            std::span{digit_9}.size() };
  static SDL_Point * digits[] = {digit_0, digit_1, digit_2, digit_3, digit_4, digit_5, digit_6, digit_7, digit_8, digit_9 };

  std::array<SDL_Point, 7> points;
  long long score = game.get_score();
  int no_of_digits = 0;
  if (score > 0) {
    no_of_digits = std::trunc( std::log10( score ) ) + 1;
  }
  size_t x = SCORE_X + 20 * no_of_digits;
  size_t y = SCORE_Y;
  
  do {
    int d = score % 10;
    score /= 10;
    size_t size = sizes[d];
    for (size_t i = 0; i < size; i++) {
      points[i].x = x +  4 * (digits[d] + i)->x;
      points[i].y = y +  4 * (digits[d] + i)->y;
    }
    x -= 20;
//    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
    SDL_RenderDrawLines(renderer, points.data(), size );
//    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    no_of_digits--;
  } while (no_of_digits > 0);
}


bool SDL2Renderer::init() {
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
  } else {
    window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN );
    if( window == nullptr ) {
      std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
      renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
      if ( renderer == nullptr )  {
        std::cout << "No renderer from window: " <<  SDL_GetError() << std::endl;
      }
      return true;
    }
  }
  return false;
}


void SDL2Renderer::render() {
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderClear( renderer );
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  
  for (Body2df * body : game.get_physics().get_bodies() ) {
    TypedBody * typed_body = static_cast<TypedBody *>(body);
    auto type = typed_body->get_type();
    if (type == BodyType::spaceship) {
      render( static_cast<Spaceship *>(typed_body) );
    } else if (type == BodyType::torpedo ) {
      render( static_cast<Torpedo *>(typed_body) );
    } else if (type == BodyType::asteroid) {
      render( static_cast<Asteroid *>(typed_body) );
    } else if (type == BodyType::spaceship_debris ) {
      render( static_cast<SpaceshipDebris *>(typed_body) );
    } else if (type == BodyType::debris) {
      render( static_cast<Debris *>(typed_body) );
    } else if (type == BodyType::saucer) {
      render( static_cast<Saucer *>(typed_body) );
    }
  }
  renderFreeShips();
  renderScore();
  SDL_RenderPresent( renderer );
}


void SDL2Renderer::exit() {
  SDL_DestroyWindow( window );
  SDL_Quit();
}
 
