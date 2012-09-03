#include <gtest/gtest.h>
#include "src/SymMatrix.hpp"

TEST(SymMatrix, constructor){
  size_t n(4);
	SymMatrix matrix(n);

	for ( size_t i=0 ; i < n ; i++ ){
	  for ( size_t j=0 ; j < n ; j++ ){
		  ASSERT_DOUBLE_EQ(0.0, matrix.get(i, j));
		}
	}
}

TEST(SymMatrix, set){
  size_t n(3);
  SymMatrix matrix(n);

  matrix.set(0, 0, 1.3);
  matrix.set(0, 1, 2.7);
  matrix.set(2, 2, -3.2);
  ASSERT_DOUBLE_EQ(1.3, matrix.get(0, 0));
  ASSERT_DOUBLE_EQ(2.7, matrix.get(0, 1));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(0, 2));
  ASSERT_DOUBLE_EQ(2.7, matrix.get(1, 0));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(1, 1));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(1, 2));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(2, 0));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(2, 1));
  ASSERT_DOUBLE_EQ(-3.2, matrix.get(2, 2));

  matrix.set(1, 0, 3.8);
  ASSERT_DOUBLE_EQ(1.3, matrix.get(0, 0));
  ASSERT_DOUBLE_EQ(3.8, matrix.get(0, 1));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(0, 2));
  ASSERT_DOUBLE_EQ(3.8, matrix.get(1, 0));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(1, 1));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(1, 2));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(2, 0));
  ASSERT_DOUBLE_EQ(0.0, matrix.get(2, 1));
  ASSERT_DOUBLE_EQ(-3.2, matrix.get(2, 2));
}
