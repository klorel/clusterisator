#include <gtest/gtest.h>
#include "src/KMConstraints.hpp"
using namespace std;

TEST(KMConstraints, usage){
  //Create the instance of KMConstraints
  size_t nbObs(10);
  KMConstraints csts(nbObs);

  ASSERT_EQ((size_t) 0, csts.size());
  ASSERT_EQ(csts.begin(), csts.end());

  //Add some constraints
  csts.newCtr(3, 4);
  csts.newCtr(4, 7);
  csts.newCtr(9, 8);
  csts.newCtr(9, 2);
  csts.newCtr(0, 9);

  //Make sure we're in the expected state
  ASSERT_EQ((size_t) 5, csts.size());
  ASSERT_EQ(IntSet({9}), csts.get(0));
  ASSERT_EQ(IntSet({}), csts.get(1));
  ASSERT_EQ(IntSet({9}), csts.get(2));
  ASSERT_EQ(IntSet({4}), csts.get(3));
  ASSERT_EQ(IntSet({3, 7}), csts.get(4));
  ASSERT_EQ(IntSet(), csts.get(5));
  ASSERT_EQ(IntSet(), csts.get(6));
  ASSERT_EQ(IntSet({4}), csts.get(7));
  ASSERT_EQ(IntSet({9}), csts.get(8));
  ASSERT_EQ(IntSet({0, 2, 8}), csts.get(9));

  //Try to clear everything
  csts.clear();
  ASSERT_EQ((size_t) 0, csts.size());
  ASSERT_EQ(csts.begin(), csts.end());
  ASSERT_EQ(IntSet(), csts.get(0));
  ASSERT_EQ(IntSet(), csts.get(1));
  ASSERT_EQ(IntSet(), csts.get(2));
  ASSERT_EQ(IntSet(), csts.get(3));
  ASSERT_EQ(IntSet(), csts.get(4));
  ASSERT_EQ(IntSet(), csts.get(5));
  ASSERT_EQ(IntSet(), csts.get(6));
  ASSERT_EQ(IntSet(), csts.get(7));
  ASSERT_EQ(IntSet(), csts.get(8));
  ASSERT_EQ(IntSet(), csts.get(9));

  //And finally add something again after the clearing
  csts.newCtr(1, 2);
  ASSERT_EQ((size_t) 1, csts.size());
  ASSERT_EQ(IntSet(), csts.get(0));
  ASSERT_EQ(IntSet({2}), csts.get(1));
  ASSERT_EQ(IntSet({1}), csts.get(2));
  ASSERT_EQ(IntSet(), csts.get(3));
  ASSERT_EQ(IntSet(), csts.get(4));
  ASSERT_EQ(IntSet(), csts.get(5));
  ASSERT_EQ(IntSet(), csts.get(6));
  ASSERT_EQ(IntSet(), csts.get(7));
  ASSERT_EQ(IntSet(), csts.get(8));
  ASSERT_EQ(IntSet(), csts.get(9));
}
