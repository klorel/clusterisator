#include <gtest/gtest.h>
#include "IndexedList.hpp"

TEST(IndexedList, contains){
  IndexedList list(4);

  EXPECT_FALSE(list.contains(0));
  EXPECT_FALSE(list.contains(1));
  EXPECT_FALSE(list.contains(2));
  EXPECT_FALSE(list.contains(3));
  list.insert(2);
  EXPECT_FALSE(list.contains(0));
  EXPECT_FALSE(list.contains(1));
  EXPECT_TRUE(list.contains(2));
  EXPECT_FALSE(list.contains(3));
}
