#include "game.h"
#include "gtest/gtest.h"


namespace {
  
TEST(SPACESHIP, InitalState) {
  Spaceship ship{ Vector2df{125.0f, 100.0f} }; 
  
  ASSERT_FALSE(ship.is_accelerating());
  EXPECT_NEAR(125.0f, ship.get_position()[0], 0.00001f);
  EXPECT_NEAR(100.0f, ship.get_position()[1], 0.00001f);
  EXPECT_NEAR(0.0f, ship.get_angle(), 0.00001f);
}
  
TEST(SPACESHIP, HalfTurn) {
  Spaceship ship{ Vector2df{125.0f, 100.0f} }; 
  
  ship.turn(PI, 0.5f);
  ship.turn(PI, 0.5f);
  EXPECT_NEAR(PI, ship.get_angle(), 0.00001f);
}
  
  
TEST(GAME, GetInitalScore) {
  Game game{}; 
  
  ASSERT_EQ(0LL, game.get_score());
}

TEST(GAME, NoInitalObjectsCreated) {
  Game game{}; 
  
  ASSERT_EQ(0, game.get_physics().get_bodies().size());
}

TEST(GAME, InitalObjectsCreated) {
  Game game{}; 
  
  game.tick(0.05f);
  game.tick(0.05f);
  ASSERT_EQ(5, game.get_physics().get_bodies().size());
}

TEST(GAME, ShipShoots) {
  Game game{}; 
  
  game.tick(0.05f);
  game.ship_shoots();
  game.tick(0.15f);
  game.ship_shoots();
  game.tick(0.15f);
  game.ship_shoots();
  game.tick(0.15f);
  game.ship_shoots();
  game.tick(0.05f);
  ASSERT_EQ(9, game.get_physics().get_bodies().size());
}


}