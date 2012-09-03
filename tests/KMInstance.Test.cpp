#include <gtest/gtest.h>
#include "src/KMInstance.hpp"
#include "src/Partition.hpp"

TEST(KMInstance, constructor){
  size_t nbObs(10), nbAtt(3);
  KMInstance instance(nbObs, nbAtt);

  ASSERT_EQ(nbObs, instance.data().getN());
  ASSERT_EQ(nbAtt, instance.data().getM());
  ASSERT_EQ(nbObs, instance.nbObs());
  ASSERT_EQ(nbAtt, instance.nbAtt());


  ASSERT_EQ(nbObs, instance.weights().size());
  for ( Double weight : instance.weights() ){
    ASSERT_DOUBLE_EQ(1.0, weight);
  }

  ASSERT_EQ((size_t) 0, instance.mustLinks().size());
  ASSERT_EQ((size_t) 0, instance.cannotLinks().size());
}

TEST(KMInstance, addLinks){
  size_t nbObs(7), nbAtt(3);
  KMInstance instance(nbObs, nbAtt);

  //Create some constraints
  instance.addMustLink(0, 3);
  instance.addMustLink(3, 2);
  instance.addMustLink(6, 5);
  instance.addCannotLink(0, 6);
  instance.addCannotLink(1, 0);

  //Make sure we have the expected "must-link" constraints
  KMConstraints mustLinks = instance.mustLinks();
  ASSERT_EQ((size_t) 3, mustLinks.size());
  ASSERT_EQ(IntSet({3}), mustLinks.get(0));
  ASSERT_EQ(IntSet(), mustLinks.get(1));
  ASSERT_EQ(IntSet({3}), mustLinks.get(2));
  ASSERT_EQ(IntSet({0, 2}), mustLinks.get(3));
  ASSERT_EQ(IntSet(), mustLinks.get(4));
  ASSERT_EQ(IntSet({6}), mustLinks.get(5));
  ASSERT_EQ(IntSet({5}), mustLinks.get(6));

  //Make sure we have the expected "cannot-link" constraints
  KMConstraints cannotLinks = instance.cannotLinks();
  ASSERT_EQ((size_t) 2, cannotLinks.size());
  ASSERT_EQ(IntSet({1, 6}), cannotLinks.get(0));
  ASSERT_EQ(IntSet({0}), cannotLinks.get(1));
  ASSERT_EQ(IntSet(), cannotLinks.get(2));
  ASSERT_EQ(IntSet(), cannotLinks.get(3));
  ASSERT_EQ(IntSet(), cannotLinks.get(4));
  ASSERT_EQ(IntSet(), cannotLinks.get(5));
  ASSERT_EQ(IntSet({0}), cannotLinks.get(6));
}

TEST(KMInstance, feasible){
  size_t nbObs(7), nbAtt(3), nbMaxLabels(4);
  KMInstance instance(nbObs, nbAtt);
  Partition partition(nbObs, nbMaxLabels); //FIXME: to have an actual unit test, use a mock instead

  //First, we don't have any constraint. Every partition should be feasible
  partition.setLabels(IntVector({0, 0, 0, 0, 0, 0, 0}));
  ASSERT_TRUE(instance.feasible(partition));

  partition.setLabels(IntVector({0, 1, 2, 2, 3, 1, 3}));
  ASSERT_TRUE(instance.feasible(partition));

  //Now, add some constraints...
  instance.addMustLink(0, 3);
  instance.addMustLink(3, 2);
  instance.addMustLink(6, 5);
  instance.addCannotLink(0, 6);
  instance.addCannotLink(1, 0);

  // ... and check some partitions
  partition.setLabels(IntVector{0, 2, 0, 0, 2, 1, 1});
  ASSERT_TRUE(instance.feasible(partition));

  partition.setLabels(IntVector{0, 0, 0, 0, 2, 1, 1}); // infeasible: 0 and 1 have the same label
  ASSERT_FALSE(instance.feasible(partition));

  partition.setLabels(IntVector{0, 2, 0, 0, 2, 1, 2}); // infeasible: 5 and 6 don't have the same label
  ASSERT_FALSE(instance.feasible(partition));

  partition.setLabels(IntVector{3, 0, 3, 3, 2, 1, 1});
  ASSERT_TRUE(instance.feasible(partition));
}
