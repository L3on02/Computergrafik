#include "geometry.h"
#include "gtest/gtest.h"

namespace {
	
TEST(RAY, ListInitialization2df) {
  Ray2df ray = { {0.0, 0.0}, {1.0, 0.0} };
  
  EXPECT_NEAR(0.0, ray.origin[0], 0.00001);
  EXPECT_NEAR(0.0, ray.origin[1], 0.00001);
  EXPECT_NEAR(1.0, ray.direction[0], 0.00001);
  EXPECT_NEAR(0.0, ray.direction[1], 0.00001);
}

TEST(AABB, Intersects2df_1) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {0.5, -0.5}, {0.5, 0.5} };
  
  EXPECT_TRUE( box1.intersects(box2) );
}

TEST(AABB, Intersects2df_2) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {2.5, -2.5}, {0.5, 0.5} };
  
  EXPECT_FALSE( box1.intersects(box2) );
}

TEST(AABB, Intersects2df_3) {
  AABB2df box1( {1.5, 1.5}, {0.5, 0.5} );
  AABB2df box2( {0.75, 1.0}, {0.75, 1.0} );

  EXPECT_TRUE( box1.intersects(box2) );
}

TEST(AABB, Intersects2dfWithRay_1) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  Ray2df ray = { {0.0, -3.0}, {1.0, 1.0} };
  
  EXPECT_FALSE( box1.intersects(ray) );
}

TEST(AABB, Intersects2dfWithRay_2) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  Ray2df ray = { {-1.0, -2.0}, {0.5, 0.5} };
  
  EXPECT_TRUE( box1.intersects(ray) );
}

TEST(AABB, Intersects2dfWithMovingAABB_1) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {-2.0, -2.0}, {0.5, .5} };
  vec2 direction = {1.0, 1.0};
  
  EXPECT_TRUE( box1.intersects(box2, direction) );
}

TEST(AABB, Intersects2dfWithMovingAABB_2) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {-2.0, -2.0}, {0.5, 0.5} };
  vec2 direction = {1.0, 0.0};
  
  EXPECT_FALSE( box1.intersects(box2, direction) );
}

TEST(AABB, Intersects2dfWithMovingAABB_3) {
  AABB2df box1 = { {2.0, 2.0}, {1.0, 1.0} };
  AABB2df box2 = { {2.0, 5.0}, {0.5, 0.5} };
  vec2 direction = {0.1, -3.0};
  
  EXPECT_TRUE( box1.intersects(box2, direction) );
}

TEST(AABB, Intersects2dfWithMovingAABB_4) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {2.0, 0.0}, {0.5, 0.5} };
  vec2 direction = {-1.0, 0.0};
  
  EXPECT_TRUE(box1.intersects(box2, direction));
}


TEST(AABB, SweepIntersects2df_1) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {-2.0, -2.0}, {0.5, .5} };
  vec2 direction = {1.0, 1.0};
  
  vec2 normal = box1.sweep_intersects(box2, direction);
  
  EXPECT_TRUE(normal[0] < 0.0);
  EXPECT_TRUE(normal[1] < 0.0);
}

TEST(AABB, SweepIntersects2df_2) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {-2.0, -2.0}, {0.5, 0.5} };
  vec2 direction = {1.0, 0.0};
  
  vec2 normal = box1.sweep_intersects(box2, direction);

  EXPECT_NEAR(0.0, normal[0], 0.00001);
  EXPECT_NEAR(0.0, normal[1], 0.00001);
}

TEST(AABB, SweepIntersects2df_3) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {-2.0, -2.0}, {0.5, 0.5} };
  vec2 direction = {1.0, 1.5};
  
  vec2 normal = box1.sweep_intersects(box2, direction);
  
  EXPECT_TRUE(normal[0] < 0.0);
  EXPECT_NEAR(0.0, normal[1], 0.00001);
}

TEST(AABB, SweepIntersects2df_4) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {2.0, 0.0}, {0.5, 0.5} };
  vec2 direction = {-1.0, 0.0};
  
  vec2 normal = box1.sweep_intersects(box2, direction);
  
  EXPECT_TRUE(normal[0] > 0.0);
  EXPECT_NEAR(0.0, normal[1], 0.00001);
}

TEST(AABB, SweepIntersects2df_5) {
  AABB2df box1 = { {0.0, 0.0}, {1.0, 1.0} };
  AABB2df box2 = { {-2.0, -2.0}, {0.5, 0.5} };
  vec2 direction = {-1.0, -1.5};
  
  vec2 normal = box2.sweep_intersects(box1, direction);
  
  EXPECT_TRUE(normal[0] > 0.0);
  EXPECT_NEAR(0.0, normal[1], 0.00001);
}

TEST(SPHERE, Intersects2dfWithSphere_1) {
  Sphere2df sphere1 = { {0.0, 0.0}, 1.0 };
  Sphere2df sphere2 = { {1.0, 1.0}, 0.5 };
  
  EXPECT_TRUE( sphere1.intersects(sphere2) );
}

TEST(SPHERE, Intersects2dfWithSphere_2) {
  Sphere2df sphere1 = { {0.0, 0.0}, 1.0 };
  Sphere2df sphere2 = { {2.0, 2.0}, 0.5 };
  
  EXPECT_FALSE( sphere1.intersects(sphere2) );
}

TEST(SPHERE, Intersects2dfWithRay_1) {
  Sphere2df sphere = { {0.0, 0.0}, 1.0 };
  Ray2df ray{ {-2.0, -3.0}, {1.0, 1.0} };
  EXPECT_NEAR(2.0, sphere.intersects(ray), 0.000001 );
}

TEST(SPHERE, Intersects2dfWithRay_2) {
  Sphere2df sphere = { {0.0, 0.0}, 1.0 };
  Ray2df ray{ {-3.0, 1.0}, {1.0, 0.0} };
  EXPECT_NEAR(3.0, sphere.intersects(ray), 0.000001 );
}

TEST(SPHERE, Intersects2dfWithRay_3) {
  Sphere2df sphere = { {1.0, 1.0}, 1.0 };
  Ray2df ray{ {4.0, 1.0}, {-1.0, 0.0} };
  EXPECT_NEAR(2.0, sphere.intersects(ray), 0.000001 );
}

TEST(SPHERE, Intersects3dfWithRay_1) {
  Sphere3df sphere = { {0.0, 0.0, 0.0}, 1.0 };
  Ray3df ray{ {-2.0, -3.0, 0.0}, {1.0, 1.0, 0.0} };
  EXPECT_NEAR(2.0, sphere.intersects(ray), 0.000001 );
}

TEST(SPHERE, Intersects3dfWithRay_2) {
  Sphere3df sphere = { {0.0, 0.0, 0.0}, 1.0 };
  Ray3df ray{ {-2.0, -3.0, 0.0}, {1.0, 1.0, 0.0} };
  Intersection_Context<float,3u> context;
  
  EXPECT_TRUE( sphere.intersects(ray, context) );
  EXPECT_NEAR( 2.0, context.t, 0.000001 );
  EXPECT_NEAR( 0.0, context.intersection[0], 0.000001 );
  EXPECT_NEAR(-1.0, context.intersection[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.intersection[2], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[0], 0.000001 );
  EXPECT_NEAR(-1.0, context.normal[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[2], 0.000001 );  
}

TEST(SPHERE, Intersects3dfWithRay_3) {
  Sphere3df sphere = { {1.0, 0.0, 0.0}, 1.0 };
  Ray3df ray{ {-1.0, -3.0, 0.0}, {1.0, 1.0, 0.0} };
  Intersection_Context<float,3u> context;
  
  EXPECT_TRUE( sphere.intersects(ray, context) );
  EXPECT_NEAR( 2.0, context.t, 0.000001 );
  EXPECT_NEAR( 1.0, context.intersection[0], 0.000001 );
  EXPECT_NEAR(-1.0, context.intersection[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.intersection[2], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[0], 0.000001 );
  EXPECT_NEAR(-1.0, context.normal[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[2], 0.000001 );  
}

TEST(SPHERE, Intersects3dfWithRay_4) {
  Sphere3df sphere = { {1.0, 0.0, 0.0}, 0.5 };
  Ray3df ray{ {1.0, 3.0, 0.0}, {0.0, -1.0, 0.0} };
  Intersection_Context<float,3u> context;
  
  EXPECT_TRUE( sphere.intersects(ray, context) );
  EXPECT_NEAR( 2.5, context.t, 0.000001 );
  EXPECT_NEAR( 1.0, context.intersection[0], 0.000001 );
  EXPECT_NEAR( 0.5, context.intersection[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.intersection[2], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[0], 0.000001 );
  EXPECT_NEAR( 1.0, context.normal[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[2], 0.000001 );  
}

TEST(SPHERE, Intersects3dfWithRay_5) {
  Sphere3df sphere = { {2.0, 0.0, 2.0}, 1.5 };
  Ray3df ray{ {3.5, 0.0, -0.5}, {0.0, 0.0, 1.0} };
  Intersection_Context<float,3u> context;
  
  EXPECT_TRUE( sphere.intersects(ray, context) );
  EXPECT_NEAR( 2.5, context.t, 0.000001 );
  EXPECT_NEAR( 3.5, context.intersection[0], 0.000001 );
  EXPECT_NEAR( 0.0, context.intersection[1], 0.000001 );
  EXPECT_NEAR( 2.0, context.intersection[2], 0.000001 );
  EXPECT_NEAR( 1.0, context.normal[0], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[1], 0.000001 );
  EXPECT_NEAR( 0.0, context.normal[2], 0.000001 );  
}

TEST(SPHERE, Intersects3dfWithRay_6) {
  Sphere3df sphere = { {-15.0f, 0.0f, 2.0f}, 10.0f };
  Ray3df ray{ {0.0f, 0.0f, 20.0f}, {0.0f, 0.0f, -15.0f} };
  Intersection_Context<float,3u> context;
  
  EXPECT_FALSE( sphere.intersects(ray, context) );
}

TEST(SPHERE, Intersects3dfWithRay_7) {
  // ray starts inside sphere
  Sphere3df sphere = { {3.0f, 3.0f, 0.0f}, 3.0f };
  Ray3df ray{ {3.5f, 3.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };
  Intersection_Context<float,3u> context;
  
  EXPECT_TRUE( sphere.intersects(ray, context) );
}

TEST(SPHERE, Inside_1) {
  Sphere3df sphere = { {3.0f, 3.0f, 0.0f}, 3.0f };
  
  EXPECT_TRUE( sphere.inside( vec3{3.5f, 3.0f, 0.0f}) );
}

TEST(SPHERE, NotInside_1) {
  Sphere3df sphere = { {3.0f, 3.0f, 0.0f}, 3.0f };
  
  EXPECT_FALSE( sphere.inside( vec3{-0.5f, 0.0f, 0.0f}) );
}

TEST(TRIANGLE, Intersects3dfWithRay_1) {
  Triangle3df triangle = { {0.0, 0.0, 0.0}, {0.0, 3.0, 0.0},{3.0, 0.0, 0.0}  };
  Ray3df ray{ {0.0, 0.0, 2.0}, {0.0, 0.0, -1.0} };
  Intersection_Context<float,3u> context;
    
  EXPECT_TRUE( triangle.intersects(ray, context) );
  EXPECT_NEAR(2.0, context.t, 0.000001 );
  EXPECT_NEAR(0.0, context.intersection[0], 0.000001 );
  EXPECT_NEAR(0.0, context.intersection[1], 0.000001 );
  EXPECT_NEAR(0.0, context.intersection[2], 0.000001 );
  EXPECT_NEAR(1.0, context.u, 0.000001 );
  EXPECT_NEAR(0.0, context.v, 0.000001 );
}

TEST(TRIANGLE, Intersects3dfWithRay_2) {
  Triangle3df triangle = { {0.0, 0.0, 0.0}, {0.0, 3.0, 0.0},{3.0, 0.0, 0.0}  };
  Ray3df ray{ {1.0, 1.0, 2.0}, {0.0, 0.0, -1.0} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(2.0, t, 0.000001 );
  EXPECT_NEAR(1.0, intersection[0], 0.000001 );
  EXPECT_NEAR(1.0, intersection[1], 0.000001 );
  EXPECT_NEAR(0.0, intersection[2], 0.000001 );
}

TEST(TRIANGLE, Intersects3dfWithRay_3) {
  Triangle3df triangle1 = { {-5.0f, -5.0f,-5.0f}, {-5.0f, 5.0f, -5.0f}, { 5.0,  5.0, -5.0} };
  Ray3df ray{ {0.0, 0.0, 20.0}, {-0.75, 0.520833, -15.0} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
}

TEST(TRIANGLE, Intersects3dfWithRay_4) {
  Triangle3df triangle1 = { {-2.0f, -1.0f, 0.0f}, {0.0f, 2.0f, 0.0f}, { 2.0, 0.0, 0.0} };
  Ray3df ray{ {0.0, 0.0, 20.0}, {0.0, 0.0, -2.0} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(10.0, t, 0.00001);
}

TEST(TRIANGLE, Intersects3dfWithRay_5) {
  Triangle3df triangle1 = { {-2.0f, -1.0f, 0.0f}, {0.0f, 2.0f, 0.0f}, { 2.0, 0.0, 0.0} };
  Ray3df ray{ {-2.0, 0.0, 2.0}, {1.0, 0.0, -1.0} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(2.0, t, 0.00001);
}

TEST(TRIANGLE, Intersects3dfWithRay_6) {
  Triangle3df triangle1 = { {0.0f, -2.0f, -1.0f}, {0.0f, 0.0f, 2.0f}, {0.0, 2.0, 0.0} };
  Ray3df ray{ {20.0, 0.0, 0.0}, {-2.0, 0.0, 0.0} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(10.0, t, 0.00001);
}

TEST(TRIANGLE, Intersects3dfWithRay_7) {
  Triangle3df triangle1 = { {2.0f, 0.0f, 0.0f}, {-2.0f, 0.0f, 2.0f}, {-2.0f, 0.0f, -2.0f} };
  Ray3df ray{ {0.0f, 20.0f, 0.0f}, {0.0f, -2.0f, 0.0f} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(10.0, t, 0.00001);
}

TEST(TRIANGLE, Intersects3dfWithRay_8) {
  Triangle3df triangle1 = { {-5.0f,  5.0f, 5.0f}, { -5.0f, 5.0f, -5.0f}, { 5.0f,  5.0f,  -5.0f}  };
  Ray3df ray{ {-3.0f, 0.0f, -3.0f}, {0.0f, 1.0f, 0.0f} };

  float u;
  float v;
  float t;
  vec3 intersection{},
            normal{};
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(5.0, t, 0.00001);
}

TEST(TRIANGLE, Intersects3dfWithRay_9) {
  Triangle3df triangle1 = { {-5.0f,  5.0f, 5.0f}, { -5.0f, 5.0f, -5.0f}, { 5.0f,  5.0f,  -5.0f}  };
  vec3 intersection = {-3.0, 5.0, -3.0};
  vec3 eye = {-2.0f, 0.0f, -2.0f};
  vec3 direction = intersection - eye;
  Ray3df ray{ eye, direction };
  vec3 normal{};

  float u;
  float v;
  float t;
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
  EXPECT_NEAR(1.0, t, 0.00001);
  EXPECT_NEAR(-3.0, intersection[0], 0.00001);
  EXPECT_NEAR(5.0, intersection[1], 0.00001);
  EXPECT_NEAR(-3.0, intersection[2], 0.00001);
}

TEST(TRIANGLE, Intersects3dfWithRay_10) {
  Triangle3df triangle1 = { {-5.0f,  5.0f, 5.0f}, { -5.0f, 5.0f, -5.0f}, { 5.0,  5.0,  -5.0}  };
  vec3 intersection = {0.0, 0.0, 0.0};
  vec3 eye = {0.0f, 0.0f, 20.0f};
  vec3 direction = {-4.08594, 4.42969, -15.0};
  Ray3df ray{ eye, direction };
  vec3 normal{};

  float u;
  float v;
  float t;
  
  EXPECT_TRUE(triangle1.intersects(ray, normal, intersection, u, v, t) );
}

TEST(FRESNEL, Refract_1) {
  vec3 eye = {0.0f, 0.0f, 0.0f};
  vec3 direction = {0.0f, -1.0f, 0.0f};
  Ray3df ray{ eye, direction };
  Intersection_Context<float, 3> context{};
  vec3 transmission{};
  
  context.normal = {0.0f, 1.0f, 0.0f};
  bool refracted = refract<float, 3>(1.0f, context.normal, ray.direction, transmission);
  EXPECT_TRUE( refracted );
  EXPECT_NEAR( 0.0f, transmission[0], 0.00001);
  EXPECT_NEAR(-1.0f, transmission[1], 0.00001);
  EXPECT_NEAR( 0.0f, transmission[2], 0.00001);
}



}
