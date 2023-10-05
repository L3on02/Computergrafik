#include "physics.h"
#include "gtest/gtest.h"


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
  Body2df body1( BoundingVolume2df({2.0, 2.0}, 1.0), {-0.5, -0.5} );
  Body2df body2( BoundingVolume2df({0.0, 0.0}, 1.0), {0.0, -1.0} );
  Body2df body3( BoundingVolume2df({0.0, -3.0}, 1.0), {0.0, 0.5} );
  BoundingVolume2df area{{5.0f, 5.0f}, 1.0f };
  Physics2df physics{};
  physics.add_body( &body1 );
  physics.add_body( &body2 );
  physics.add_body( &body3 );
  physics.tick(0.01f);

  EXPECT_TRUE( physics.is_area_free_of_bodies( &area ) );
}

TEST(PHYSICS, IsAreaFreeOfBodiesFalse) {
  Body2df body1( BoundingVolume2df({2.0, 2.0}, 1.0), {-0.5, -0.5} );
  BoundingVolume2df area{{0.0f, 0.0f}, 10.0f };
  Physics2df physics{};
  physics.add_body( &body1 );
  physics.tick();
  
  EXPECT_FALSE( physics.is_area_free_of_bodies( &area ) );
}


TEST(PHYSICS, TickCheckMovement) {
  Body2df body1( BoundingVolume2df({2.0, 2.0}, 1.0), {-0.5, -0.5} );
  Body2df body2( BoundingVolume2df({0.0, 0.0}, 1.0), {0.0, -1.0} );
  Body2df body3( BoundingVolume2df({0.0, -3.0}, 1.0), {0.0, 0.5} );
  Physics2df physics;
  physics.add_body( &body1 );
  physics.add_body( &body2 );
  physics.add_body( &body3 );
  physics.tick(1.0);
  EXPECT_NEAR(1.5, body1.get_position()[0], 0.00001);
  EXPECT_NEAR(1.5, body1.get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, body2.get_position()[0], 0.00001);
  EXPECT_NEAR(-1.0, body2.get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, body3.get_position()[0], 0.00001);
  EXPECT_NEAR(-2.5, body3.get_position()[1], 0.00001);
}

TEST(PHYSICS, TickCheckCollision) {
  bool collision_ok = false;
  Body2df body1( BoundingVolume2df({2.0, 2.0}, 1.0), {-0.5, -0.5} );
  Body2df body2( BoundingVolume2df({0.0, 0.0}, 1.0), {0.0, -1.0} );
  Body2df body3( BoundingVolume2df({0.0, -3.0}, 1.0), {0.0, 0.5} );
  std::function<bool(Body2df *, Body2df *)> check_collision =
                    [&](Body2df * , Body2df * ) ->  bool { return true; };
  std::function<void(Body2df *, Body2df *)> resolve_collision =
                    [&](Body2df * b1, Body2df * b2) ->  void { collision_ok = (b1 == &body2 && b2 == &body3); };
  Physics2df physics(check_collision, resolve_collision);
  physics.add_body( &body1 );
  physics.add_body( &body2 );
  physics.add_body( &body3 );
  physics.tick(1.0); // only one collision between body1 and body3 has to be reported
  EXPECT_EQ(3, physics.get_bodies().size());
  EXPECT_NEAR(1.5, body1.get_position()[0], 0.00001);
  EXPECT_NEAR(1.5, body1.get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, body2.get_position()[0], 0.00001);
  EXPECT_NEAR(-1.0, body2.get_position()[1], 0.00001);
  EXPECT_NEAR(0.0, body3.get_position()[0], 0.00001);
  EXPECT_NEAR(-2.5, body3.get_position()[1], 0.00001);
  EXPECT_TRUE(collision_ok);
}

TEST(PHYSICS, TickBodiesDeleted) {
  Body2df body1( BoundingVolume2df({2.0, 2.0}, 1.0), {-0.5, -0.5} );
  Body2df body2( BoundingVolume2df({0.0, 0.0}, 1.0), {0.0, -1.0} );
  Body2df body3( BoundingVolume2df({0.0, -3.0}, 1.0), {0.0, 0.5} );
  body1.set_time_to_delete(1.0);
  body2.set_time_to_delete(1.5);
  body3.set_time_to_delete(3.1);
  Physics2df physics{};
  physics.add_body( &body1 );
  physics.add_body( &body2 );
  physics.add_body( &body3 );
  physics.tick(1.0);
  physics.tick(1.0);
  auto bodies = physics.get_bodies();
  EXPECT_TRUE( body1.is_marked_for_deletion() );
  EXPECT_TRUE( body2.is_marked_for_deletion() );
  EXPECT_FALSE( body3.is_marked_for_deletion() );
  EXPECT_EQ(1, bodies.size() );
  EXPECT_EQ(&body3, bodies[0]);
}

TEST(PHYSICS, TickBodiesDeletedWhenMarkedForDeletion) {
  Body2df body1( BoundingVolume2df({2.0, 2.0}, 1.0), {-0.5, -0.5} );
  Body2df body2( BoundingVolume2df({0.0, 0.0}, 1.0), {0.0, -1.0} );
  Body2df body3( BoundingVolume2df({0.0, -3.0}, 1.0), {0.0, 0.5} );
  body1.set_time_to_delete(1.0);
  body3.set_time_to_delete(2.1);
  Physics2df physics{};
  physics.add_body( &body1 );
  physics.add_body( &body2 );
  physics.add_body( &body3 );
  physics.tick(1.0);
  body2.mark_for_deletion();
  physics.tick(1.0);
  auto bodies = physics.get_bodies();
  EXPECT_TRUE( body1.is_marked_for_deletion() );
  EXPECT_TRUE( body2.is_marked_for_deletion() );
  EXPECT_FALSE( body3.is_marked_for_deletion() );
  EXPECT_EQ(1, bodies.size() );
  EXPECT_EQ(&body3, bodies[0]);
}


// object moves 768 units (pixel) from 0 up withing 2 s and 60 FPS 
TEST(PHYSICS, TickTime60) {
  float tick_time = 1.0 / 60.0; // 60 FPS
  float velocity = 768.0 / 2.0; // m per s
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), Vector2df{0.0, velocity}, velocity );
  Physics2df physics{};
  physics.add_body( &body );
  for (size_t i = 0; i < 120; i++) {
    physics.tick(tick_time);
  }
  EXPECT_NEAR(768.0, std::round(body.get_position()[1]), 0.00001);
}

// object moves 768 units (pixel) from 0 up withing 2 s and 120 FPS 
TEST(PHYSICS, TickTime120) {
  float tick_time = 1.0 / 120.0; // 120 FPS
  float velocity = 768.0 / 2.0; // m per s
  Body2df body( BoundingVolume2df({0.0, 0.0}, 1.0), Vector2df{0.0, velocity}, velocity );
  Physics2df physics{};
  physics.add_body( &body );
  for (size_t i = 0; i < 240; i++) {
    physics.tick(tick_time);
  }
  EXPECT_NEAR(768.0, std::round(body.get_position()[1]), 0.00001);
}

}
