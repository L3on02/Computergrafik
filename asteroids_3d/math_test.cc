#include "math.h"
#include "gtest/gtest.h"

namespace
{

  TEST(VECTOR, ListInitialization2df)
  {
    vec2 vector = {1.0, 0.0};

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(0.0, vector[1], 0.00001);
  }

  TEST(VECTOR, ListInitialization3df)
  {
    vec3 vector = {1.0, 0.0, 5.0};

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(0.0, vector[1], 0.00001);
    EXPECT_NEAR(5.0, vector[2], 0.00001);
  }

  TEST(VECTOR, ListInitialization4df)
  {
    vec4 vector = {1.0, 0.0, 5.0, -5.0};

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(0.0, vector[1], 0.00001);
    EXPECT_NEAR(5.0, vector[2], 0.00001);
    EXPECT_NEAR(-5.0, vector[3], 0.00001);
  }

  TEST(VECTOR, ListInitialization4df_2)
  {
    vec4 vector = {1.0, 2.0, 3.0, 4.0};

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(2.0, vector[1], 0.00001);
    EXPECT_NEAR(3.0, vector[2], 0.00001);
    EXPECT_NEAR(4.0, vector[3], 0.00001);
  }

  TEST(VECTOR, ListInitializationSizeToSmall)
  {
    vec4 vector = {
        1.0,
        2.0,
        3.0,
    };

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(2.0, vector[1], 0.00001);
    EXPECT_NEAR(3.0, vector[2], 0.00001);
    EXPECT_NEAR(3.0, vector[3], 0.00001);
  }

  TEST(VECTOR, EmptyListInitialization)
  {
    vec4 vector = {};

    EXPECT_NEAR(0.0, vector[0], 0.00001);
    EXPECT_NEAR(0.0, vector[1], 0.00001);
    EXPECT_NEAR(0.0, vector[2], 0.00001);
    EXPECT_NEAR(0.0, vector[3], 0.00001);
  }

  TEST(VECTOR, UnitVectorWithAngle)
  {
    vec2 vector(0.0f);

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(0.0, vector[1], 0.00001);
  }

  TEST(VECTOR, UnitVectorWithAngle90)
  {
    vec2 vector(PI / 2.0f);

    EXPECT_NEAR(0.0, vector[0], 0.00001);
    EXPECT_NEAR(1.0, vector[1], 0.00001);
  }

  TEST(VECTOR, CopyConstructor)
  {
    vec2 vector = {1.0, 0.0};
    vec2 copy(vector);
    EXPECT_NEAR(1.0, copy[0], 0.00001);
    EXPECT_NEAR(0.0, copy[1], 0.00001);
  }

  TEST(VECTOR, SquareOfLength1)
  {
    vec2 vector = {2.0, 2.0};

    EXPECT_NEAR(8.0, vector.square_of_length(), 0.00001);
  }

  TEST(VECTOR, SquareOfLength3df)
  {
    vec3 vector = {4.0, 0.0, 3.0};

    EXPECT_NEAR(25.0, vector.square_of_length(), 0.00001);
  }

  TEST(VECTOR, Length)
  {
    vec2 vector = {-3.0, 4.0};

    EXPECT_NEAR(5.0, vector.length(), 0.00001);
  }

  TEST(VECTOR, Length3df)
  {
    vec3 vector = {0.0, -4.0, 3.0};
    float length = vector.length();

    EXPECT_NEAR(5.0, length, 0.00001);
  }

  TEST(VECTOR, Normalize)
  {
    vec2 vector = {-3.0, 4.0};

    vector.normalize();
    EXPECT_NEAR(1.0, vector.length(), 0.00001);
  }

  TEST(VECTOR, Normalize3df)
  {
    vec3 vector = {-3.0, 4.0, 7.8};

    vector.normalize();
    EXPECT_NEAR(1.0, vector.length(), 0.00001);
  }

  TEST(VECTOR, Normalize4df)
  {
    vec4 vector = {-3.5, 7.5, 0.001, 4.0};

    vector.normalize();
    EXPECT_NEAR(1.0, vector.length(), 0.00001);
  }

  TEST(VECTOR, GetReflective1)
  {
    vec2 vector = {1.0, -1.0};
    vec2 normal = {0.0, 1.0};

    vec2 reflectiv = vector.get_reflective(normal);

    EXPECT_NEAR(1.0, reflectiv[0], 0.00001);
    EXPECT_NEAR(1.0, reflectiv[1], 0.00001);
  }

  TEST(VECTOR, GetReflective2)
  {
    vec2 vector = {0.0, -1.0};
    vec2 normal = {1.0, 1.0};

    normal.normalize();

    vec2 reflectiv = vector.get_reflective(normal);

    EXPECT_NEAR(1.0, reflectiv[0], 0.00001);
    EXPECT_NEAR(0.0, reflectiv[1], 0.00001);
  }

  TEST(VECTOR, GetReflective3df_1)
  {
    vec3 vector = {0.0, 1.0, -1.0};
    vec3 normal = {0.0, 0.0, 1.0};

    vec3 reflectiv = vector.get_reflective(normal);

    EXPECT_NEAR(0.0, reflectiv[0], 0.00001);
    EXPECT_NEAR(1.0, reflectiv[1], 0.00001);
    EXPECT_NEAR(1.0, reflectiv[2], 0.00001);
  }

  TEST(VECTOR, Angle90)
  {
    vec2 vector{0.0f, 1.0f};

    EXPECT_NEAR(PI / 2.0f, vector.angle(0, 1), 0.00001);
  }

  TEST(VECTOR, Angle180)
  {
    vec2 vector{-1.0f, 0.0f};

    EXPECT_NEAR(PI, vector.angle(0, 1), 0.00001);
  }

  TEST(VECTOR, Angle270)
  {
    vec2 vector{0.0f, -1.0f};

    EXPECT_NEAR(-PI / 2.0f, vector.angle(0, 1), 0.00001);
  }

  TEST(VECTOR, Angle0)
  {
    vec2 vector(0.0f);

    EXPECT_NEAR(0.0f, vector.angle(0, 1), 0.00001);
  }

  TEST(VECTOR, SumsTwoVectors)
  {
    vec2 vector = {1.0, 0.0};
    vec2 addend = {-2.0, 1.0};
    vec2 sum = vector + addend;

    EXPECT_NEAR(1.0, vector[0], 0.00001);
    EXPECT_NEAR(0.0, vector[1], 0.00001);
    EXPECT_NEAR(-1.0, sum[0], 0.00001);
    EXPECT_NEAR(1.0, sum[1], 0.00001);
    EXPECT_NEAR(-2.0, addend[0], 0.00001);
    EXPECT_NEAR(1.0, addend[1], 0.00001);
  }

  TEST(VECTOR, SumsTwoVectors3df)
  {
    vec3 vector = {0.0, 1.0, 0.0};
    vec3 addend = {0.0, -2.0, 1.0};
    vec3 sum = vector + addend;

    EXPECT_NEAR(0.0, sum[0], 0.00001);
    EXPECT_NEAR(-1.0, sum[1], 0.00001);
    EXPECT_NEAR(1.0, sum[2], 0.00001);
  }

  TEST(VECTOR, AddToVector)
  {
    vec2 vector = {0.1, 0.5};
    vec2 addend = {0.0, 0.5};
    vector += addend;

    EXPECT_NEAR(0.1, vector[0], 0.00001);
    EXPECT_NEAR(1.0, vector[1], 0.00001);
  }

  TEST(VECTOR, ScalarProduct)
  {
    vec2 vector1 = {1.0, 0.0};
    vec2 vector2 = 2.0f * vector1;

    EXPECT_NEAR(2.0, vector2[0], 0.00001);
    EXPECT_NEAR(0.0, vector2[1], 0.00001);
  }

  TEST(VECTOR, ScalarProduct3df)
  {
    vec3 vector1 = {0.0, 1.0, 0.0};
    vec3 vector2 = 2.0f * vector1;

    EXPECT_NEAR(0.0, vector1[0], 0.00001);
    EXPECT_NEAR(1.0, vector1[1], 0.00001);
    EXPECT_NEAR(0.0, vector1[2], 0.00001);
    EXPECT_NEAR(0.0, vector2[0], 0.00001);
    EXPECT_NEAR(2.0, vector2[1], 0.00001);
    EXPECT_NEAR(0.0, vector2[2], 0.00001);
  }

  TEST(VECTOR, ScalarAssignmentProduct)
  {
    vec2 vector1 = {1.0, 0.0};
    vector1 *= 2.0;

    EXPECT_NEAR(2.0, vector1[0], 0.00001);
    EXPECT_NEAR(0.0, vector1[1], 0.00001);
  }

  TEST(VECTOR, ScalarAssignmentDivision)
  {
    vec2 vector1 = {1.0, 0.0};
    vector1 /= 0.5;

    EXPECT_NEAR(2.0, vector1[0], 0.00001);
    EXPECT_NEAR(0.0, vector1[1], 0.00001);
  }

  TEST(VECTOR, ScalarVectorProduct1)
  {
    vec2 vector1 = {1.0, 0.0};
    vec2 vector2 = {0.0, 1.0};

    EXPECT_NEAR(0.0, vector1 * vector2, 0.00001);
  }

  TEST(VECTOR, ScalarVectorProduct2)
  {
    vec3 vector1 = {1.0, 2.0, -1.0};
    vec3 vector2 = {-1.0, 1.0, 3.0};

    float scalar = vector1 * vector2;

    EXPECT_NEAR(-2.0, scalar, 0.00001);
    EXPECT_NEAR(1.0, vector1[0], 0.00001);
    EXPECT_NEAR(2.0, vector1[1], 0.00001);
    EXPECT_NEAR(-1.0, vector1[2], 0.00001);
    EXPECT_NEAR(-1.0, vector2[0], 0.00001);
    EXPECT_NEAR(1.0, vector2[1], 0.00001);
    EXPECT_NEAR(3.0, vector2[2], 0.00001);
  }

  TEST(VECTOR, ScalarVectorProduct3df_1)
  {
    vec3 vector1 = {0.0, 1.0, 0.0};
    vec3 vector2 = {0.0, 0.0, 1.0};

    EXPECT_NEAR(0.0, vector1 * vector2, 0.00001);
  }

  TEST(VECTOR, ScalarVectorProduct3df_2)
  {
    vec3 vector1 = {-1.0, 2.0, 3.0};
    vec3 vector2 = {2.0, 2.0, -1.0};

    EXPECT_NEAR(-1.0, vector1 * vector2, 0.00001);
  }

  TEST(VECTOR, ScalarVectorProduct3df_3)
  {
    vec3 vector1 = {0.0, -2.0, 0.0};
    vec3 vector2 = {0.0, -10.0, 0.0};

    EXPECT_NEAR(20.0, vector1 * vector2, 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct1)
  {
    vec3 vector1 = {1.0, 0.0, 0.0};
    vec3 vector2 = {0.0, 1.0, 0.0};
    vec3 cross = vector1.cross_product(vector2);

    EXPECT_NEAR(0.0, cross[0], 0.00001);
    EXPECT_NEAR(0.0, cross[1], 0.00001);
    EXPECT_NEAR(1.0, cross[2], 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct2)
  {
    vec3 vector1 = {-2.0, 1.0, -2.0};
    vec3 vector2 = {-3.0, 3.0, 0.0};
    vec3 cross = vector1.cross_product(vector2);

    EXPECT_NEAR(-2.0, vector1[0], 0.00001);
    EXPECT_NEAR(1.0, vector1[1], 0.00001);
    EXPECT_NEAR(-2.0, vector1[2], 0.00001);
    EXPECT_NEAR(-3.0, vector2[0], 0.00001);
    EXPECT_NEAR(3.0, vector2[1], 0.00001);
    EXPECT_NEAR(0.0, vector2[2], 0.00001);
    EXPECT_NEAR(6.0, cross[0], 0.00001);
    EXPECT_NEAR(-6.0, cross[1], 0.00001);
    EXPECT_NEAR(-3.0, cross[2], 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct3)
  {
    vec3 vector1 = {-1.0, 0.0, -4.0};
    vec3 vector2 = {2.0, 0.0, -2.0};
    vec3 cross = vector1.cross_product(vector2);

    EXPECT_NEAR(-1.0, vector1[0], 0.00001);
    EXPECT_NEAR(0.0, vector1[1], 0.00001);
    EXPECT_NEAR(-4.0, vector1[2], 0.00001);
    EXPECT_NEAR(2.0, vector2[0], 0.00001);
    EXPECT_NEAR(0.0, vector2[1], 0.00001);
    EXPECT_NEAR(-2.0, vector2[2], 0.00001);
    EXPECT_NEAR(0.0, cross[0], 0.00001);
    EXPECT_NEAR(10.0, cross[1], 0.00001);
    EXPECT_NEAR(0.0, cross[2], 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct4)
  {
    vec3 vector1 = {-1.0, 0.0, -4.0};
    vec3 vector2 = {2.0, 0.0, -2.0};

    vec3 cross = vector2.cross_product(vector1);

    EXPECT_NEAR(-1.0, vector1[0], 0.00001);
    EXPECT_NEAR(0.0, vector1[1], 0.00001);
    EXPECT_NEAR(-4.0, vector1[2], 0.00001);
    EXPECT_NEAR(2.0, vector2[0], 0.00001);
    EXPECT_NEAR(0.0, vector2[1], 0.00001);
    EXPECT_NEAR(-2.0, vector2[2], 0.00001);
    EXPECT_NEAR(0.0, cross[0], 0.00001);
    EXPECT_NEAR(-10.0, cross[1], 0.00001);
    EXPECT_NEAR(0.0, cross[2], 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct5)
  {
    vec3 a = {-1.0, 0.0, -2.0};
    vec3 b = {2.0, 0.0, 0.0};
    vec3 c = {0.0, 0.0, 2.0};
    vec3 ab = b - a;
    vec3 ac = c - a;

    vec3 cross = ab.cross_product(ac);

    EXPECT_NEAR(3.0, ab[0], 0.00001);
    EXPECT_NEAR(0.0, ab[1], 0.00001);
    EXPECT_NEAR(2.0, ab[2], 0.00001);

    EXPECT_NEAR(1.0, ac[0], 0.00001);
    EXPECT_NEAR(0.0, ac[1], 0.00001);
    EXPECT_NEAR(4.0, ac[2], 0.00001);

    EXPECT_NEAR(0.0, cross[0], 0.00001);
    EXPECT_NEAR(10.0, cross[1], 0.00001);
    EXPECT_NEAR(0.0, cross[2], 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct6)
  {
    vec3 vector1 = {1.0, 0.0, 0.0};
    vec3 vector2 = {0.0, 0.0, 1.0};
    vec3 cross = vector1.cross_product(vector2);

    EXPECT_NEAR(0.0, cross[0], 0.00001);
    EXPECT_NEAR(1.0, cross[1], 0.00001);
    EXPECT_NEAR(0.0, cross[2], 0.00001);
  }

  TEST(VECTOR, CrossVectorProduct7)
  {
    vec3 vector1 = {0.0, 1.0, 0.0};
    vec3 vector2 = {0.0, 0.0, 1.0};
    vec3 cross = vector1.cross_product(vector2);

    EXPECT_NEAR(1.0, cross[0], 0.00001);
    EXPECT_NEAR(0.0, cross[1], 0.00001);
    EXPECT_NEAR(0.0, cross[2], 0.00001);
  }

  // eigene Tests
  TEST(VectorTest, Length)
  {
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

  TEST(VectorMultiplicationTest, MultiplicationOfTwoVectors)
  {
    Vector<float, 3> vector1{1.0, 2.0, 3.0};
    Vector<float, 3> vector2{4.0, 5.0, 6.0};
    float expected_result = 32.0;
    float actual_result = vector1 * vector2;
    EXPECT_EQ(expected_result, actual_result);
  }

}