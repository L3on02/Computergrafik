#include "physics.h"
#include "gtest/gtest.h"
#include <memory>

namespace {
	
TEST(BOUNDING_VOLUME, DoesNotCollide) {
  BoundingVolume2df boundingVolume1( {0.0, 0.0}, 1.0 );
  BoundingVolume2df boundingVolume2( {2.0, 2.0}, 0.5 );
  
  EXPECT_FALSE( boundingVolume1.collides(boundingVolume2) );
}

TEST(BOUNDING_VOLUME, Collides) {
  BoundingVolume2df boundingVolume1( {0.0, 0.0}, 1.0 );
  BoundingVolume2df boundingVolume2( {1.0, 1.0}, 1.0 );
  
  EXPECT_TRUE( boundingVolume1.collides(boundingVolume2) );
}

TEST(BOUNDING_VOLUME, CollidesInside) {
  BoundingVolume2df boundingVolume1( {0.0, 0.0}, 10.0 );
  BoundingVolume2df boundingVolume2( {1.0, 1.0}, 1.0 );
  
  EXPECT_TRUE( boundingVolume1.collides(boundingVolume2) );
}

TEST(BOUNDING_VOLUME, CollidesInside2) {
  BoundingVolume2df boundingVolume1( {2.0, 2.0}, 1.0 );
  BoundingVolume2df boundingVolume2( {0.0f, 0.0f}, 10.0f );
  
  EXPECT_TRUE( boundingVolume1.collides(boundingVolume2) );
}


TEST(RECT_BOUNDING_VOLUME, DoesNotCollide) {
  Rectangle2df boundingVolume1( {0.0, 0.0}, {1.0, 1.0} );
  Rectangle2df boundingVolume2( {2.0, 2.0}, {0.5, 0.5} );
  
  EXPECT_FALSE( boundingVolume1.collides(boundingVolume2) );
}

TEST(RECT_BOUNDING_VOLUME, Collides1) {
  Rectangle2df boundingVolume1( {0.0, 0.0}, {1.0, 1.0} );
  Rectangle2df boundingVolume2( {0.5, 0.5}, {1.0, 1.0} );
  
  EXPECT_TRUE( boundingVolume1.collides(boundingVolume2) );
}

TEST(RECT_BOUNDING_VOLUME, Collides2) {
  Rectangle2df boundingVolume1( {1.0, 1.0}, {1.0, 1.0} );
  Rectangle2df boundingVolume2( {0.0, 0.0}, {1.5, 2.0} );
  
  EXPECT_TRUE( boundingVolume1.collides(boundingVolume2) );
}

TEST(RECT_BOUNDING_VOLUME, CollidesInside) {
  Rectangle2df boundingVolume1( {1.0, 1.0}, {3.0, 3.0} );
  Rectangle2df boundingVolume2( {2.0, 2.0}, {0.5, 0.5} );
  
  EXPECT_TRUE( boundingVolume1.collides(boundingVolume2) );
}


TEST(BODY, Move) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.move();
  EXPECT_NEAR(1.0, body.get_position()[0], 0.00001);
  EXPECT_NEAR(0.0, body.get_position()[1], 0.00001);
}

TEST(BODY, IsMarkedForDeletion) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.mark_for_deletion();
  EXPECT_TRUE(body.is_marked_for_deletion());
}

TEST(BODY, IsSetTimeForDeletionFalse) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.set_time_to_delete(1.0);
  body.move(0.7f);
  EXPECT_FALSE(body.is_marked_for_deletion());
}

TEST(BODY, TimeSetTimeToDelete) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.set_time_to_delete(1.0);
  EXPECT_FALSE(body.is_marked_for_deletion());
}

TEST(BODY, TimeSetTimeToDeleteAndMove) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.set_time_to_delete(1.0);
  body.move(1.1);
  EXPECT_TRUE(body.is_marked_for_deletion());
}


TEST(BODY, Bounce0) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.bounce(0);
  EXPECT_NEAR(-1.0, body.get_velocity()[0], 0.00001);
}

TEST(BODY, Bounce1) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {0.0, 1.0} );
  body.bounce(1);
  EXPECT_NEAR(-1.0, body.get_velocity()[1], 0.00001);
}

TEST(BODY, BounceBounce0) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {-1.5, 0.0} );
  body.bounce(0);
  body.bounce(0);
  EXPECT_NEAR(-1.5, body.get_velocity()[0], 0.00001);
}


TEST(BODY, AccelerateAndMove) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0} );
  body.accelerate(-0.5);
  body.move();
  EXPECT_NEAR(0.5, body.get_position()[0], 0.00001);
  EXPECT_NEAR(0.0, body.get_position()[1], 0.00001);
}

TEST(BODY, AccelerateAndMoveWithMinimumVelocity) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0}, 10.0, 1.0 );
  body.accelerate(-0.5);
  body.move();
  EXPECT_NEAR(1.0, body.get_position()[0], 0.00001);
  EXPECT_NEAR(0.0, body.get_position()[1], 0.00001);
}

TEST(BODY, AccelerateAndMoveWithMaximumVelocity) {
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), {1.0, 0.0}, 2.0);
  body.accelerate(0.5);
  body.accelerate(0.5);
  body.accelerate(0.5);
  body.move();
  EXPECT_NEAR(2.0, body.get_position()[0], 0.00001);
  EXPECT_NEAR(0.0, body.get_position()[1], 0.00001);
}

TEST(PHYSICS, IsAreaFreeOfBodiesTrue) {
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>( BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  std::unique_ptr<Body2df> body2 = std::make_unique<Body2df>( BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, -1.0} );
  std::unique_ptr<Body2df> body3 = std::make_unique<Body2df>( BoundingVolume2df({0.0, -3.0}, 1.0), vec2{0.0, 0.5} );
  BoundingVolume2df area{{5.0f, 5.0f}, 1.0f };
  Physics2df physics{};
  physics.add_body( body1 );
  physics.add_body( body2 );
  physics.add_body( body3 );
  physics.tick(0.01f);

  EXPECT_TRUE( physics.is_area_free_of_bodies( &area ) );
}

TEST(PHYSICS, IsAreaFreeOfBodiesFalse) {
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>(BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  BoundingVolume2df area{{0.0f, 0.0f}, 10.0f };
  Physics2df physics{};
  physics.add_body( body1 );
  physics.tick();
  
  EXPECT_FALSE( physics.is_area_free_of_bodies( &area ) );
}

TEST(PHYSICS, TickCheckMovement) {
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>( BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  std::unique_ptr<Body2df> body2 = std::make_unique<Body2df>( BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, -1.0} );
  std::unique_ptr<Body2df> body3 = std::make_unique<Body2df> ( BoundingVolume2df({0.0, -3.0}, 1.0), vec2{0.0, 0.5} );
  Body2df * b1 = body1.get();
  Body2df * b2 = body2.get();
  Body2df * b3 = body3.get();
  Physics2df physics;
  physics.add_body( body1 );
  physics.add_body( body2 );
  physics.add_body( body3 );
  physics.tick(1.0);
  EXPECT_NEAR(1.5, b1->get_position()[0], 0.00001);
  EXPECT_NEAR(1.5, b1->get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, b2->get_position()[0], 0.00001);
  EXPECT_NEAR(-1.0, b2->get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, b3->get_position()[0], 0.00001);
  EXPECT_NEAR(-2.5, b3->get_position()[1], 0.00001);
}

TEST(PHYSICS, TickCheckCollision) {
  bool collision_ok = false;
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>( BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  std::unique_ptr<Body2df> body2 = std::make_unique<Body2df>( BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, -1.0} );
  std::unique_ptr<Body2df> body3 = std::make_unique<Body2df>( BoundingVolume2df({0.0, -3.0}, 1.0), vec2{0.0, 0.5} );
  Body2df * b1 = body1.get();
  Body2df * b2 = body2.get();
  Body2df * b3 = body3.get();
  std::function<bool(Body2df *, Body2df *)> check_collision =
                    [&](Body2df * , Body2df * ) ->  bool { return true; };
  std::function<void(Body2df *, Body2df *)> resolve_collision =
                    [&](Body2df * body_1, Body2df * body_2) ->  void { collision_ok = (body_1 == b2 && body_2 == b3); };
  Physics2df physics(check_collision, resolve_collision);
  physics.add_body( body1 );
  physics.add_body( body2 );
  physics.add_body( body3 );
  physics.tick(1.0); // only one collision between body2 and body3 has to be reported
  EXPECT_EQ(3, physics.get_bodies().size());
  EXPECT_NEAR(1.5, b1->get_position()[0], 0.00001);
  EXPECT_NEAR(1.5, b1->get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, b2->get_position()[0], 0.00001);
  EXPECT_NEAR(-1.0, b2->get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, b3->get_position()[0], 0.00001);
  EXPECT_NEAR(-2.5, b3->get_position()[1], 0.00001);
  EXPECT_TRUE(collision_ok);
}

TEST(PHYSICS, TickBodiesDeletedWhenMarkedForDeletion) {
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>( BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  std::unique_ptr<Body2df> body2 = std::make_unique<Body2df>( BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, -1.0} );
  std::unique_ptr<Body2df> body3 = std::make_unique<Body2df>( BoundingVolume2df({0.0, -3.0}, 1.0), vec2{0.0, 0.5} );
  body1->set_time_to_delete(1.0);
  body3->set_time_to_delete(2.1);
  Body2df * b1 = body1.get();
  Body2df * b2 = body2.get();
  Body2df * b3 = body3.get();
  Physics2df physics{};
  physics.add_body( body1 );
  physics.add_body( body2 );
  physics.add_body( body3 );
  physics.tick(1.0);
  b2->mark_for_deletion();
  physics.tick(1.0);
  EXPECT_TRUE( b1->is_marked_for_deletion() );
  EXPECT_TRUE( b2->is_marked_for_deletion() );
  EXPECT_FALSE( b3->is_marked_for_deletion() );
  EXPECT_EQ(1, physics.get_bodies().size() );
  EXPECT_EQ(b3, physics.get_body(0));
}


TEST(PHYSICS, GetRecentlyAddedBodies) {
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>( BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  Body2df * b1 = body1.get();
  Physics2df physics;
  physics.add_body( body1 );
  physics.tick(1.0);
  auto added = physics.get_recently_added_bodies();
    
  EXPECT_EQ(1, added.size());
  EXPECT_EQ(b1, added[0]);
}


TEST(PHYSICS, GetBodies) {
  std::unique_ptr<Body2df> body1 = std::make_unique<Body2df>( BoundingVolume2df({2.0, 2.0}, 1.0), vec2{-0.5, -0.5} );
  std::unique_ptr<Body2df> body2 = std::make_unique<Body2df>( BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, -1.0} );
  std::unique_ptr<Body2df> body3 = std::make_unique<Body2df>( BoundingVolume2df({0.0, -3.0}, 1.0), vec2{0.0, 0.5} );
  Body2df * b1 = body1.get();
  Body2df * b2 = body2.get();
  Body2df * b3 = body3.get();
  Physics2df physics{};
  physics.add_body( body1 );
  physics.add_body( body2 );
  physics.add_body( body3 );
  physics.tick(1.0);
  const std::vector< std::unique_ptr< Body2df> >   & bodies = physics.get_bodies();
  EXPECT_EQ(3, bodies.size() );
  EXPECT_EQ(b1, bodies[0].get());
  EXPECT_EQ(b2, bodies[1].get());
  EXPECT_EQ(b3, bodies[2].get());
}


// object moves 768 units (pixel) from 0 up withing 2 s and 60 FPS 
TEST(PHYSICS, TickTime60) {
  float tick_time = 1.0 / 60.0; // 60 FPS
  float velocity = 768.0 / 2.0; // m per s
  std::unique_ptr<Body2df>  body = std::make_unique<Body2df>( BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, velocity}, velocity );
  Body2df * b = body.get();
  Physics2df physics{};
  physics.add_body( body );
  for (size_t i = 0; i < 120; i++) {
    physics.tick(tick_time);
  }
  EXPECT_NEAR(768.0, std::round(b->get_position()[1]), 0.00001);
}


// object moves 768 units (pixel) from 0 up withing 2 s and 120 FPS 
TEST(PHYSICS, TickTime120) {
  float tick_time = 1.0 / 120.0; // 120 FPS
  float velocity = 768.0 / 2.0; // m per s
  std::unique_ptr<Body2df>  body = std::make_unique<Body2df>(BoundingVolume2df({0.0, 0.0}, 1.0), vec2{0.0, velocity}, velocity );
  Body2df * b = body.get();  Physics2df physics{};
  physics.add_body( body );
  for (size_t i = 0; i < 240; i++) {
    physics.tick(tick_time);
  }
  EXPECT_NEAR(768.0, std::round(b->get_position()[1]), 0.00001);
}

}
