#include "math.h"
#include "gtest/gtest.h"

namespace {
	
TEST(VECTOR, ListInitialization2df) {
  Vector2df vector = {1.0, 0.0};
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(0.0, vector[1], 0.00001);
}

TEST(VECTOR, ListInitialization3df) {
  Vector3df vector = {1.0, 0.0, 5.0};
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(0.0, vector[1], 0.00001);
  EXPECT_NEAR(5.0, vector[2], 0.00001);
}


TEST(VECTOR, ListInitialization4df) {
  Vector4df vector = {1.0, 0.0, 5.0, -5.0};
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(0.0, vector[1], 0.00001);
  EXPECT_NEAR(5.0, vector[2], 0.00001);
  EXPECT_NEAR(-5.0, vector[3], 0.00001);
}

TEST(VECTOR, ListInitialization4df_2) {
  Vector4df vector = {1.0, 2.0, 3.0, 4.0};
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(2.0, vector[1], 0.00001);
  EXPECT_NEAR(3.0, vector[2], 0.00001);
  EXPECT_NEAR(4.0, vector[3], 0.00001);
}

TEST(VECTOR, ListInitializationSizeToSmall) {
  Vector4df vector = {1.0, 2.0, 3.0, };
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(2.0, vector[1], 0.00001);
  EXPECT_NEAR(3.0, vector[2], 0.00001);
  EXPECT_NEAR(3.0, vector[3], 0.00001);
}

TEST(VECTOR, EmptyListInitialization) {
  Vector4df vector = {};
  
  EXPECT_NEAR(0.0, vector[0], 0.00001);
  EXPECT_NEAR(0.0, vector[1], 0.00001);
  EXPECT_NEAR(0.0, vector[2], 0.00001);
  EXPECT_NEAR(0.0, vector[3], 0.00001);
}

TEST(VECTOR, UnitVectorWithAngle) {
  Vector2df vector(0.0f);
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(0.0, vector[1], 0.00001);
}

TEST(VECTOR, UnitVectorWithAngle90) {
  Vector2df vector(PI / 2.0f);
  
  EXPECT_NEAR(0.0, vector[0], 0.00001);
  EXPECT_NEAR(1.0, vector[1], 0.00001);
}


TEST(VECTOR, CopyConstructor) {
  Vector2df vector = {1.0, 0.0};
  Vector2df copy(vector);
  EXPECT_NEAR(1.0, copy[0], 0.00001);
  EXPECT_NEAR(0.0, copy[1], 0.00001);
}

TEST(VECTOR, SquareOfLength1) {
  Vector2df vector = {2.0, 2.0};
  
  EXPECT_NEAR(8.0, vector.square_of_length(), 0.00001);
}

TEST(VECTOR, SquareOfLength3df) {
  Vector3df vector = {4.0, 0.0, 3.0};
  
  EXPECT_NEAR(25.0, vector.square_of_length(), 0.00001);
}

TEST(VECTOR, Length) {
  Vector2df vector = {-3.0, 4.0};
  
  EXPECT_NEAR(5.0, vector.length(), 0.00001);
}

TEST(VECTOR, Length3df) {
  Vector3df vector = {0.0, -4.0, 3.0};
  float length = vector.length();
    
  EXPECT_NEAR(5.0, length, 0.00001);
}

TEST(VECTOR, Normalize) {
  Vector2df vector = {-3.0, 4.0};
  
  vector.normalize();
  EXPECT_NEAR(1.0, vector.length(), 0.00001);
}

TEST(VECTOR, Normalize3df) {
  Vector3df vector = {-3.0, 4.0, 7.8};
  
  vector.normalize();
  EXPECT_NEAR(1.0, vector.length(), 0.00001);
}

TEST(VECTOR, Normalize4df) {
  Vector4df vector = {-3.5, 7.5, 0.001, 4.0};
  
  vector.normalize();
  EXPECT_NEAR(1.0, vector.length(), 0.00001);
}

TEST(VECTOR, GetReflective1) {
  Vector2df vector = {1.0, -1.0};
  Vector2df normal = {0.0, 1.0};
  
  Vector2df reflectiv = vector.get_reflective(normal);
  
  EXPECT_NEAR(1.0, reflectiv[0], 0.00001);
  EXPECT_NEAR(1.0, reflectiv[1], 0.00001);
}

TEST(VECTOR, GetReflective2) {
  Vector2df vector = {0.0, -1.0};
  Vector2df normal = {1.0, 1.0};
  
  normal.normalize();
  
  Vector2df reflectiv = vector.get_reflective(normal);
  
  EXPECT_NEAR(1.0, reflectiv[0], 0.00001);
  EXPECT_NEAR(0.0, reflectiv[1], 0.00001);
}

TEST(VECTOR, GetReflective3df_1) {
  Vector3df vector = {0.0, 1.0, -1.0};
  Vector3df normal = {0.0, 0.0, 1.0};
  
  Vector3df reflectiv = vector.get_reflective(normal);
  
  EXPECT_NEAR(0.0, reflectiv[0], 0.00001);
  EXPECT_NEAR(1.0, reflectiv[1], 0.00001);
  EXPECT_NEAR(1.0, reflectiv[2], 0.00001);
}

TEST(VECTOR, Angle90) {
  Vector2df vector{ 0.0f, 1.0f};
  
  EXPECT_NEAR(PI / 2.0f, vector.angle(0,1), 0.00001);
}

TEST(VECTOR, Angle180) {
  Vector2df vector{ -1.0f, 0.0f};
  
  EXPECT_NEAR(PI, vector.angle(0,1), 0.00001);
}

TEST(VECTOR, Angle270) {
  Vector2df vector{ 0.0f, -1.0f};
  
  EXPECT_NEAR(-PI / 2.0f, vector.angle(0,1), 0.00001);
}

TEST(VECTOR, Angle0) {
  Vector2df vector(0.0f);
  
  EXPECT_NEAR(0.0f, vector.angle(0,1), 0.00001);
}

TEST(VECTOR, SumsTwoVectors) {
  Vector2df vector = {1.0, 0.0};
  Vector2df addend = {-2.0, 1.0};
  Vector2df sum = vector + addend;
  
  EXPECT_NEAR(1.0, vector[0], 0.00001);
  EXPECT_NEAR(0.0, vector[1], 0.00001);
  EXPECT_NEAR(-1.0, sum[0], 0.00001);
  EXPECT_NEAR(1.0, sum[1], 0.00001);
  EXPECT_NEAR(-2.0, addend[0], 0.00001);
  EXPECT_NEAR(1.0, addend[1], 0.00001);
}

TEST(VECTOR, SumsTwoVectors3df) {
  Vector3df vector = {0.0, 1.0, 0.0};
  Vector3df addend = {0.0, -2.0, 1.0};
  Vector3df sum = vector + addend;
  
  EXPECT_NEAR( 0.0, sum[0], 0.00001);
  EXPECT_NEAR(-1.0, sum[1], 0.00001);
  EXPECT_NEAR( 1.0, sum[2], 0.00001);
}


TEST(VECTOR, AddToVector) {
  Vector2df vector = {0.1, 0.5};
  Vector2df addend = {0.0, 0.5};
  vector += addend;
  
  EXPECT_NEAR(0.1, vector[0], 0.00001);
  EXPECT_NEAR(1.0, vector[1], 0.00001);
}

TEST(VECTOR, ScalarProduct) {
  Vector2df vector1 = {1.0, 0.0};
  Vector2df vector2 = 2.0f * vector1;
  
  EXPECT_NEAR(2.0, vector2[0], 0.00001);
  EXPECT_NEAR(0.0, vector2[1], 0.00001);
}

TEST(VECTOR, ScalarProduct3df) {
  Vector3df vector1 = {0.0, 1.0, 0.0};
  Vector3df vector2 = 2.0f * vector1;
  
  EXPECT_NEAR(0.0, vector1[0], 0.00001);
  EXPECT_NEAR(1.0, vector1[1], 0.00001);
  EXPECT_NEAR(0.0, vector1[2], 0.00001);
  EXPECT_NEAR(0.0, vector2[0], 0.00001);
  EXPECT_NEAR(2.0, vector2[1], 0.00001);
  EXPECT_NEAR(0.0, vector2[2], 0.00001);
}


TEST(VECTOR, ScalarAssignmentProduct) {
  Vector2df vector1 = {1.0, 0.0};
  vector1 *= 2.0;
  
  EXPECT_NEAR(2.0, vector1[0], 0.00001);
  EXPECT_NEAR(0.0, vector1[1], 0.00001);
}

TEST(VECTOR, ScalarAssignmentDivision) {
  Vector2df vector1 = {1.0, 0.0};
  vector1 /= 0.5;
  
  EXPECT_NEAR(2.0, vector1[0], 0.00001);
  EXPECT_NEAR(0.0, vector1[1], 0.00001);
}

TEST(VECTOR, ScalarVectorProduct1) {
  Vector2df vector1 = {1.0, 0.0};
  Vector2df vector2 = {0.0, 1.0};
  
  EXPECT_NEAR(0.0, vector1 * vector2, 0.00001);
}

TEST(VECTOR, ScalarVectorProduct2) {
  Vector3df vector1 = {1.0, 2.0, -1.0};
  Vector3df vector2 = {-1.0, 1.0, 3.0};

  float scalar = vector1 * vector2;

  EXPECT_NEAR(-2.0, scalar, 0.00001);
  EXPECT_NEAR(1.0, vector1[0], 0.00001);
  EXPECT_NEAR(2.0,  vector1[1], 0.00001);
  EXPECT_NEAR(-1.0, vector1[2], 0.00001);
  EXPECT_NEAR(-1.0, vector2[0], 0.00001);
  EXPECT_NEAR(1.0,  vector2[1], 0.00001);
  EXPECT_NEAR(3.0, vector2[2], 0.00001);
}

TEST(VECTOR, ScalarVectorProduct3df_1) {
  Vector3df vector1 = {0.0, 1.0, 0.0};
  Vector3df vector2 = {0.0, 0.0, 1.0};
  
  EXPECT_NEAR(0.0, vector1 * vector2, 0.00001);
}

TEST(VECTOR, ScalarVectorProduct3df_2) {
  Vector3df vector1 = {-1.0, 2.0, 3.0};
  Vector3df vector2 = { 2.0, 2.0, -1.0};
  
  EXPECT_NEAR(-1.0, vector1 * vector2, 0.00001);
}

TEST(VECTOR, ScalarVectorProduct3df_3) {
  Vector3df vector1 = {0.0,  -2.0, 0.0};
  Vector3df vector2 = {0.0, -10.0, 0.0};
  
  EXPECT_NEAR(20.0, vector1 * vector2, 0.00001);
}

TEST(VECTOR, CrossVectorProduct1) {
  Vector3df vector1 = {1.0, 0.0, 0.0};
  Vector3df vector2 = {0.0, 1.0, 0.0};
  Vector3df cross = vector1.cross_product(vector2);
  
  EXPECT_NEAR(0.0, cross[0], 0.00001);
  EXPECT_NEAR(0.0, cross[1], 0.00001);
  EXPECT_NEAR(1.0, cross[2], 0.00001);
}

TEST(VECTOR, CrossVectorProduct2) {
  Vector3df vector1 = {-2.0, 1.0, -2.0};
  Vector3df vector2 = {-3.0, 3.0, 0.0};
  Vector3df cross = vector1.cross_product(vector2);
  
  EXPECT_NEAR(-2.0, vector1[0], 0.00001);
  EXPECT_NEAR(1.0,  vector1[1], 0.00001);
  EXPECT_NEAR(-2.0, vector1[2], 0.00001);
  EXPECT_NEAR(-3.0, vector2[0], 0.00001);
  EXPECT_NEAR(3.0,  vector2[1], 0.00001);
  EXPECT_NEAR(0.0, vector2[2], 0.00001);
  EXPECT_NEAR(6.0, cross[0], 0.00001);
  EXPECT_NEAR(-6.0,  cross[1], 0.00001);
  EXPECT_NEAR(-3.0, cross[2], 0.00001);
}

TEST(VECTOR, CrossVectorProduct3) {
  Vector3df vector1 = {-1.0, 0.0, -4.0};
  Vector3df vector2 = {2.0, 0.0, -2.0};
  Vector3df cross = vector1.cross_product(vector2);
  
  EXPECT_NEAR(-1.0, vector1[0], 0.00001);
  EXPECT_NEAR(0.0,  vector1[1], 0.00001);
  EXPECT_NEAR(-4.0, vector1[2], 0.00001);
  EXPECT_NEAR(2.0, vector2[0], 0.00001);
  EXPECT_NEAR(0.0,  vector2[1], 0.00001);
  EXPECT_NEAR(-2.0, vector2[2], 0.00001);
  EXPECT_NEAR(0.0, cross[0], 0.00001);
  EXPECT_NEAR(10.0,  cross[1], 0.00001);
  EXPECT_NEAR(0.0, cross[2], 0.00001);
}

TEST(VECTOR, CrossVectorProduct4) {
  Vector3df vector1 = {-1.0, 0.0, -4.0};
  Vector3df vector2 = {2.0, 0.0, -2.0};
  
  Vector3df cross = vector2.cross_product(vector1);
  
  EXPECT_NEAR(-1.0, vector1[0], 0.00001);
  EXPECT_NEAR(0.0,  vector1[1], 0.00001);
  EXPECT_NEAR(-4.0, vector1[2], 0.00001);
  EXPECT_NEAR(2.0, vector2[0], 0.00001);
  EXPECT_NEAR(0.0,  vector2[1], 0.00001);
  EXPECT_NEAR(-2.0, vector2[2], 0.00001);
  EXPECT_NEAR(0.0, cross[0], 0.00001);
  EXPECT_NEAR(-10.0,  cross[1], 0.00001);
  EXPECT_NEAR(0.0, cross[2], 0.00001);
}

TEST(VECTOR, CrossVectorProduct5) {
  Vector3df a = {-1.0, 0.0, -2.0};
  Vector3df b = { 2.0, 0.0, 0.0};
  Vector3df c = { 0.0, 0.0, 2.0};
  Vector3df ab = b - a;
  Vector3df ac = c - a;
  
  Vector3df cross = ab.cross_product(ac);

  EXPECT_NEAR(3.0, ab[0], 0.00001);
  EXPECT_NEAR(0.0, ab[1], 0.00001);
  EXPECT_NEAR(2.0, ab[2], 0.00001);

  EXPECT_NEAR(1.0, ac[0], 0.00001);
  EXPECT_NEAR(0.0, ac[1], 0.00001);
  EXPECT_NEAR(4.0, ac[2], 0.00001);

  
  EXPECT_NEAR(0.0,  cross[0], 0.00001);
  EXPECT_NEAR(10.0, cross[1], 0.00001);
  EXPECT_NEAR(0.0,  cross[2], 0.00001);
}

TEST(VECTOR, CrossVectorProduct6) {
  Vector3df vector1 = {1.0, 0.0, 0.0};
  Vector3df vector2 = {0.0, 0.0, 1.0};
  Vector3df cross = vector1.cross_product(vector2);
  
  EXPECT_NEAR(0.0, cross[0], 0.00001);
  EXPECT_NEAR(1.0, cross[1], 0.00001);
  EXPECT_NEAR(0.0, cross[2], 0.00001);
}

TEST(VECTOR, CrossVectorProduct7) {
  Vector3df vector1 = {0.0, 1.0, 0.0};
  Vector3df vector2 = {0.0, 0.0, 1.0};
  Vector3df cross = vector1.cross_product(vector2);
  
  EXPECT_NEAR(1.0, cross[0], 0.00001);
  EXPECT_NEAR(0.0, cross[1], 0.00001);
  EXPECT_NEAR(0.0, cross[2], 0.00001);
}

}

// eigene Tests
TEST(VectorTest, Length) {
  Vector<float, 3> v1 = {1.0f, 2.0f, 2.0f};
  float length = v1.length();
  EXPECT_FLOAT_EQ(length, 3.0f);
}

TEST(VectorTest, SquareOfLengthTest)
{
    Vector<float, 3> v1{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v1.square_of_length(), 14.0f);

    Vector<float, 4> v2{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v2.square_of_length(), 30.0f);
}

TEST(VectorMultiplicationTest, MultiplicationOfTwoVectors) {
  Vector<float, 3> vector1{1.0, 2.0, 3.0};
  Vector<float, 3> vector2{4.0, 5.0, 6.0};
  float expected_result = 32.0;
  float actual_result = vector1 * vector2;
  EXPECT_EQ(expected_result, actual_result);
}