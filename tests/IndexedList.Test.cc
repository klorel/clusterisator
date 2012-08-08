#include <gtest/gtest.h>
#include <set>
#include "IndexedList.hpp"

TEST(IndexedList, containsAndErase){
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

  list.insert(1);
  EXPECT_FALSE(list.contains(0));
  EXPECT_TRUE(list.contains(1));
  EXPECT_TRUE(list.contains(2));
  EXPECT_FALSE(list.contains(3));

  list.erase(1);
  EXPECT_FALSE(list.contains(0));
  EXPECT_FALSE(list.contains(1));
  EXPECT_TRUE(list.contains(2));
  EXPECT_FALSE(list.contains(3));
}

TEST(IndexedList, sizes){
  size_t size(5);
  IndexedList list(size);

  EXPECT_EQ(list.size(), (size_t) 0);
  EXPECT_EQ(list.max_size(), size);

  list.insert(0);
  EXPECT_EQ(list.size(), (size_t) 1);
  EXPECT_EQ(list.max_size(), size);

  list.insert(1);
  EXPECT_EQ(list.size(), (size_t) 2);
  EXPECT_EQ(list.max_size(), size);

  list.erase(1);
  EXPECT_EQ(list.size(), (size_t) 1);
  EXPECT_EQ(list.max_size(), size);
}

TEST(IndexedList, fill){
  IndexedList list(4);

  EXPECT_FALSE(list.contains(0));
  EXPECT_FALSE(list.contains(1));
  EXPECT_FALSE(list.contains(2));
  EXPECT_FALSE(list.contains(3));
  list.fill();
  EXPECT_TRUE(list.contains(0));
  EXPECT_TRUE(list.contains(1));
  EXPECT_TRUE(list.contains(2));
  EXPECT_TRUE(list.contains(3));

  IndexedList prefilledList(4, true);
  EXPECT_TRUE(prefilledList.contains(0));
  EXPECT_TRUE(prefilledList.contains(1));
  EXPECT_TRUE(prefilledList.contains(2));
  EXPECT_TRUE(prefilledList.contains(3));

  IndexedList partiallyFilledList(4);
  partiallyFilledList.insert(2);
  EXPECT_FALSE(partiallyFilledList.contains(0));
  EXPECT_FALSE(partiallyFilledList.contains(1));
  EXPECT_TRUE(partiallyFilledList.contains(2));
  EXPECT_FALSE(partiallyFilledList.contains(3));

  list.fill();
  EXPECT_TRUE(list.contains(0));
  EXPECT_TRUE(list.contains(1));
  EXPECT_TRUE(list.contains(2));
  EXPECT_TRUE(list.contains(3));
}

TEST(IndexedList, random){
  IndexedList list(5);

  //Insert some elements in the list and keep track of them
  std::set<size_t> items;
  items.insert(0);
  items.insert(3);
  items.insert(4);
  for ( size_t item : items ){
    list.insert(item);
  }
  //Make sure we start in an exepected state
  EXPECT_EQ(items.size(), list.size());

  //Make sure random() returns an element actually added
  EXPECT_TRUE(items.find(list.random()) != items.end());

  //And Make sure the items are still in the list
  EXPECT_EQ(items.size(), list.size());
}

TEST(IndexedList, pop_random){
  IndexedList list(5);

  //Insert some elements in the list and keep track of them
  std::set<size_t> items;
  items.insert(0);
  items.insert(3);
  items.insert(4);
  for ( size_t item : items ){
    list.insert(item);
  }
  //Make sure we start in an exepected state
  EXPECT_EQ(items.size(), list.size());

  //Make sure pop_random() returns an element actually added
  size_t extracted = list.pop_random();
  EXPECT_TRUE(items.find(extracted) != items.end());

  //Make sure we kept every item except the extracted one
  EXPECT_EQ(items.size()-1, list.size());
  for ( size_t item : items ){
    if ( item == extracted ){
      EXPECT_FALSE(list.contains(item));
    } else {
      EXPECT_TRUE(list.contains(item));
    }
  }
}

TEST(IndexedList, clear){
  IndexedList list(4);
  list.insert(3);
  list.insert(2);

  EXPECT_EQ((size_t) 2, list.size());
  list.clear();
  EXPECT_EQ((size_t) 0, list.size());
}

TEST(IndexedList, front){
  IndexedList list(4);

  list.insert(2);
  EXPECT_EQ((size_t) 2, list.front());
  EXPECT_EQ((size_t) 2, *(list.begin()));

  list.insert(1);
  EXPECT_EQ((size_t) 1, list.front());
  EXPECT_EQ((size_t) 1, *(list.begin()));

  list.insert(3);
  EXPECT_EQ((size_t) 3, list.front());
  EXPECT_EQ((size_t) 3, *(list.begin()));
}

