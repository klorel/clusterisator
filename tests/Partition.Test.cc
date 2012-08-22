#include <gtest/gtest.h>
#include <map>
#include <set>
#include <iostream>
#include "src/Partition.hpp"

TEST(Partition, initialState){
  size_t nbObs(6);
  size_t nbMaxLabels(3);

  Partition partition(nbObs, nbMaxLabels);

 EXPECT_EQ(nbObs, partition.nbObs());
 EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());

 //When the partition is built, every observation has the same label...
 EXPECT_EQ((Double) 1, partition.nbLabels());

 for ( size_t obs=0 ; obs < nbObs ; obs++ ){
   EXPECT_EQ(1, partition.obsWeight(obs));
 }

 EXPECT_EQ(nbObs, partition.labelWeight(0));
 EXPECT_EQ(nbObs, partition.sizeOfLabel(0));
 EXPECT_TRUE(partition.isUsed(0));
 EXPECT_FALSE(partition.allLabelsUsed());
 for ( size_t label=1 ; label < nbMaxLabels ; label++ ){
   EXPECT_EQ(0, partition.labelWeight(label));
   EXPECT_EQ((size_t) 0, partition.sizeOfLabel(label));
   EXPECT_FALSE(partition.isUsed(label));
 }


 for ( size_t obs=0 ; obs < nbObs ; obs++ ){
   EXPECT_EQ((size_t) 0, partition.label(obs));
 }
}

/**
 * Helper function which make s
 */
void  expect_eq(const std::map<size_t, size_t>& expected, size_t expectedMaxNbLabels, const Partition& partition){
  //check the number of observations
  EXPECT_EQ(expected.size(), partition.nbObs());
  EXPECT_EQ(expectedMaxNbLabels, partition.maxNbLabels());
  std::set<size_t> usedLabels;

  for ( size_t obs=0 ; obs < partition.nbObs() ; obs++ ){
    size_t expectedLabel = expected.find(obs)->second;

    //Make sure every observation is associated to its expected label
    EXPECT_EQ(expectedLabel, partition.label(obs));

    //Mark this label as used to make other checks later
    usedLabels.insert(expectedLabel);
  }

  for ( size_t label=0 ; label < expectedMaxNbLabels ; label++ ){
    //Make sure the relevant labels are correctly flaged as used (and vice versa)
    EXPECT_EQ(usedLabels.find(label) != usedLabels.end(), partition.isUsed(label));
  }

  EXPECT_EQ(usedLabels.size(), partition.nbLabels());
}

TEST(Partition, shift){
  size_t nbObs(6);
  size_t nbMaxLabels(3);

  std::map<size_t, size_t> expected;

  Partition partition(nbObs, nbMaxLabels);
  for ( size_t obs=0 ; obs < nbObs ; obs++ ){
    // At the beginning, every observation has the label 0
    expected[obs] = 0;
  }

  partition.shift(1, 2);
  expected[1] = 2;
  expect_eq(expected, nbMaxLabels, partition);

  partition.shift(4, 1);
  expected[4] = 1;
  expect_eq(expected, nbMaxLabels, partition);

  partition.shift(3, 2);
  expected[3] = 2;
  expect_eq(expected, nbMaxLabels, partition);

  partition.shift(5, 0);
  expected[5] = 0;
  expect_eq(expected, nbMaxLabels, partition);

  partition.shift(1, 1);
  expected[1] = 1;
  expect_eq(expected, nbMaxLabels, partition);

  partition.shift(0, 1);
  expected[0] = 1;
  expect_eq(expected, nbMaxLabels, partition);

  partition.shift(3, 0);
  expected[3] = 0;
  expect_eq(expected, nbMaxLabels, partition);
}
