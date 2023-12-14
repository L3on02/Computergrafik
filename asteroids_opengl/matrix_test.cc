#include "matrix.h"
#include "gtest/gtest.h"
#include <iostream>

namespace
{

  TEST(MATRIX, ListInitialization2df)
  {
    sm2 matrix = {vec2{1.0, 0.0},
                  vec2{0.0, 1.0}};

    EXPECT_NEAR(1.0, matrix.at(0, 0), 0.00001);
    EXPECT_NEAR(0.0, matrix.at(0, 1), 0.00001);
    EXPECT_NEAR(0.0, matrix.at(1, 0), 0.00001);
    EXPECT_NEAR(1.0, matrix.at(1, 1), 0.00001);
  }

  TEST(MATRIX, ListInitialization3df)
  {
    sm3 matrix = {vec3{1.0, 0.0, 0.0},
                  vec3{0.0, 1.0, 0.0},
                  vec3{0.0, 0.0, 1.0}};

    EXPECT_NEAR(1.0, matrix[0][0], 0.00001);
    EXPECT_NEAR(0.0, matrix[0][1], 0.00001);
    EXPECT_NEAR(0.0, matrix[0][2], 0.00001);
    EXPECT_NEAR(0.0, matrix[1][0], 0.00001);
    EXPECT_NEAR(1.0, matrix[1][1], 0.00001);
    EXPECT_NEAR(0.0, matrix[1][2], 0.00001);
    EXPECT_NEAR(0.0, matrix[2][0], 0.00001);
    EXPECT_NEAR(0.0, matrix[2][1], 0.00001);
    EXPECT_NEAR(1.0, matrix[2][2], 0.00001);
  }

  TEST(MATRIX, ListInitialization4df)
  {
    sm4 matrix = {{1.0, 5.0, 9.0, 13.0},
                  {2.0, 6.0, 10.0, 14.0},
                  {3.0, 7.0, 11.0, 15.0},
                  {4.0, 8.0, 12.0, 16.0}};

    float v = 1.0f;
    for (size_t row = 0; row < 4; row++)
    {
      for (size_t column = 0; column < 4; column++)
      {
        EXPECT_NEAR(v++, matrix.at(row, column), 0.00001);
      }
    }
  }

  TEST(MATRIX, ProductWithvec3)
  {
    sm3 matrix = {{1.0, 0.0, 1.0},
                  {0.0, 1.0, 1.0},
                  {0.0, 0.0, 1.0}};
    vec3 vector = {-6.0, 3.0, 1.0};
    vec3 product = matrix * vector;

    EXPECT_NEAR(-6.0, product[0], 0.00001);
    EXPECT_NEAR(3.0, product[1], 0.00001);
    EXPECT_NEAR(-2.0, product[2], 0.00001);
  }

  TEST(MATRIX, ProductWithMatrix2df)
  {
    sm2 matrix1 = {{1.0, 2.0},
                   {-1.0, 1.5}};
    sm2 matrix2 = {{2.0, -1.0},
                   {1.0, 0.0}};
    sm2 matrix = matrix1 * matrix2;

    EXPECT_NEAR(3.0, matrix.at(0, 0), 0.00001);
    EXPECT_NEAR(1.0, matrix.at(0, 1), 0.00001);
    EXPECT_NEAR(2.5, matrix.at(1, 0), 0.00001);
    EXPECT_NEAR(2.0, matrix.at(1, 1), 0.00001);
  }

  // own tests
  TEST(MatrixMultiplicationTest, MultiplySquareMatrices1)
  {
    // Arrange
    SquareMatrix<float, 3> matrix1 = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};

    SquareMatrix<float, 3> matrix2 = {{10, 13, 16}, {11, 14, 17}, {12, 15, 18}};

    SquareMatrix<float, 3> expected = {{84, 201, 318}, {90, 216, 342}, {96, 231, 366}};

    // Act
    SquareMatrix<float, 3> result = matrix1 * matrix2;

    // Assert
    int n = 3;
    for(int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        EXPECT_NEAR(expected.at(i, j), result.at(i, j), 0.00001);
      }
    }
  }
}
