#include <gtest/gtest.h>
#include "src/RectMatrix.hpp"
#include <string>
#include <vector>
using namespace std;

TEST(RectMatrix, constructor){
  /* We define the matrix
   * ( 1.1 3.2 4.7 )
   * ( 3.8 0.0 -1.6)
   * and we make sure we actually have it
   */
  size_t n(2), m(3);
  vector<Double> v({1.1, 3.2, 4.7, 3.8, 0.0, -1.6});
  RectMatrix matrix(n, m, v);

  ASSERT_EQ(n, matrix.getN());
  ASSERT_EQ(m, matrix.getM());

  EXPECT_DOUBLE_EQ(v[0], matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(v[1], matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(v[2], matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(v[3], matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(v[4], matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(v[5], matrix.get(1, 2));
}

TEST(RectMatrix, setAndGetRef){
  size_t n(2), m(3);
  vector<Double> v({1.1, 3.2, 4.7, 3.8, 0.0, -1.6});
  RectMatrix matrix(n, m, v);

  matrix.set(1, 0, 3.3);
  matrix.set(0, 0, 9.7);
  matrix.get(1, 1) = 3.8;

  ASSERT_EQ(n, matrix.getN());
  ASSERT_EQ(m, matrix.getM());

  EXPECT_DOUBLE_EQ(9.7, matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(v[1], matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(v[2], matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(3.3, matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(3.8, matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(v[5], matrix.get(1, 2));
}

TEST(RectMatrix, plus){
  size_t n(2), m(3);
  vector<Double> v({1.1, 3.2, 4.7, 3.8, 0.0, -1.6});
  RectMatrix matrix(n, m, v);

  matrix.plus(1, 0, 3.3);
  matrix.plus(0, 0, 9.7);
  matrix.plus(1, 1, 3.8);

  EXPECT_DOUBLE_EQ(v[0]+9.7, matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(v[1], matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(v[2], matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(v[3]+3.3, matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(v[4]+3.8, matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(v[5], matrix.get(1, 2));
}


TEST(RectMatrix, assign){
  size_t n(2), m(3);
  vector<Double> v({1.1, 3.2, 4.7, 3.8, 0.0, -1.6});
  RectMatrix matrix(n, m, v);

  Double value = 3.3;
  matrix.assign(value);

  EXPECT_DOUBLE_EQ(value, matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(value, matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(value, matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(value, matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(value, matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(value, matrix.get(1, 2));
}

TEST(RectMatrix, uniformConstructor){
  size_t n(2), m(3);
  Double value = 17.4;
  RectMatrix matrix(n, m, value);

  EXPECT_DOUBLE_EQ(value, matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(value, matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(value, matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(value, matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(value, matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(value, matrix.get(1, 2));

  matrix = RectMatrix(n, m);

  EXPECT_DOUBLE_EQ(0.0, matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(0.0, matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(0.0, matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(0.0, matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(0.0, matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(0.0, matrix.get(1, 2));
}

TEST(RectMatrix, read){
  string s("1.2 3.4 5.6 7.8 9.0 17.1");
  size_t n(2), m(3);
  RectMatrix matrix(n, m);
  matrix.read(s);

  EXPECT_DOUBLE_EQ(1.2, matrix.get(0, 0));
  EXPECT_DOUBLE_EQ(3.4, matrix.get(0, 1));
  EXPECT_DOUBLE_EQ(5.6, matrix.get(0, 2));
  EXPECT_DOUBLE_EQ(7.8, matrix.get(1, 0));
  EXPECT_DOUBLE_EQ(9.0, matrix.get(1, 1));
  EXPECT_DOUBLE_EQ(17.1, matrix.get(1, 2));
}
