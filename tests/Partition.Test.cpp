#include <gtest/gtest.h>
#include <map>
#include <set>
#include <vector>
#include "src/Partition.hpp"
using namespace std;

TEST(Partition, initialState) {
	size_t nbObs(6);
	size_t nbMaxLabels(3);

	Partition partition(nbObs, nbMaxLabels);

	EXPECT_EQ(nbObs, partition.nbObs());
	EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());

	//When the partition is built, every observation has the same label...
	EXPECT_EQ((Double) 1, partition.nbLabels());

	for (size_t obs = 0; obs < nbObs; obs++) {
		EXPECT_EQ(1, partition.obsWeight(obs));
	}

	EXPECT_EQ(nbObs, partition.labelWeight(0));
	EXPECT_EQ(nbObs, partition.sizeOfLabel(0));
	EXPECT_TRUE(partition.isUsed(0));
	EXPECT_FALSE(partition.allLabelsUsed());
	for (size_t label = 1; label < nbMaxLabels; label++) {
		EXPECT_EQ(0, partition.labelWeight(label));
		EXPECT_EQ((size_t) 0, partition.sizeOfLabel(label));
		EXPECT_FALSE(partition.isUsed(label));
	}

	for (size_t obs = 0; obs < nbObs; obs++) {
		EXPECT_EQ((size_t) 0, partition.label(obs));
	}
}

/**
 * Helper function which make s
 */
void expect_eq(const std::map<size_t, size_t>& expected,
		size_t expectedMaxNbLabels, const Partition& partition) {
	//check the number of observations
	EXPECT_EQ(expected.size(), partition.nbObs());
	EXPECT_EQ(expectedMaxNbLabels, partition.maxNbLabels());
	std::set<size_t> usedLabels;

	for (size_t obs = 0; obs < partition.nbObs(); obs++) {
		size_t expectedLabel = expected.find(obs)->second;

		//Make sure every observation is associated to its expected label
		EXPECT_EQ(expectedLabel, partition.label(obs));

		//Mark this label as used to make other checks later
		usedLabels.insert(expectedLabel);
	}

	for (size_t label = 0; label < expectedMaxNbLabels; label++) {
		//Make sure the relevant labels are correctly flaged as used (and vice versa)
		EXPECT_EQ(usedLabels.find(label) != usedLabels.end(),
				partition.isUsed(label));
	}

	EXPECT_EQ(usedLabels.size(), partition.nbLabels());
}

TEST(Partition, shift) {
	size_t nbObs(6);
	size_t nbMaxLabels(3);

	std::map<size_t, size_t> expected;

	Partition partition(nbObs, nbMaxLabels);
	for (size_t obs = 0; obs < nbObs; obs++) {
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

TEST(Partition, fusion) {
	size_t nbObs(6);
	size_t nbMaxLabels(5);

	Partition partition(nbObs, nbMaxLabels);
	partition.setWeights(DoubleVector( { 1, 2, 1, 4, 3, 2 }));
	partition.setLabels(IntVector( { 0, 1, 1, 2, 0, 4 }));

	//Make sure we're in the correct initial state
	EXPECT_EQ((size_t) 0, partition.label(0));
	EXPECT_EQ((size_t) 1, partition.label(1));
	EXPECT_EQ((size_t) 1, partition.label(2));
	EXPECT_EQ((size_t) 2, partition.label(3));
	EXPECT_EQ((size_t) 0, partition.label(4));
	EXPECT_EQ((size_t) 4, partition.label(5));

	EXPECT_EQ(4, partition.labelWeight(0));
	EXPECT_EQ(3, partition.labelWeight(1));
	EXPECT_EQ(4, partition.labelWeight(2));
	EXPECT_EQ(0, partition.labelWeight(3));
	EXPECT_EQ(2, partition.labelWeight(4));

	EXPECT_EQ((size_t)2, partition.sizeOfLabel(0));
	EXPECT_EQ((size_t)2, partition.sizeOfLabel(1));
	EXPECT_EQ((size_t)1, partition.sizeOfLabel(2));
	EXPECT_EQ((size_t)0, partition.sizeOfLabel(3));
	EXPECT_EQ((size_t)1, partition.sizeOfLabel(4));

	EXPECT_TRUE(partition.isUsed(0));
	EXPECT_TRUE(partition.isUsed(1));
	EXPECT_TRUE(partition.isUsed(2));
	EXPECT_FALSE(partition.isUsed(3));
	EXPECT_TRUE(partition.isUsed(4));

	EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());
	EXPECT_EQ(nbObs, partition.nbObs());

	//Start with a standard merge
	size_t label1 = partition.fusion(0, 1);
	size_t labelEmptied1 = (label1 == 0 ? 1 : 0);
	EXPECT_TRUE(label1==0 || label1==1);

	EXPECT_EQ((size_t) label1, partition.label(0));
	EXPECT_EQ((size_t) label1, partition.label(1));
	EXPECT_EQ((size_t) label1, partition.label(2));
	EXPECT_EQ((size_t) 2, partition.label(3));
	EXPECT_EQ((size_t) label1, partition.label(4));
	EXPECT_EQ((size_t) 4, partition.label(5));

	EXPECT_EQ(4+3, partition.labelWeight(label1));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied1));
	EXPECT_EQ(4, partition.labelWeight(2));
	EXPECT_EQ(0, partition.labelWeight(3));
	EXPECT_EQ(2, partition.labelWeight(4));

	EXPECT_EQ((size_t) 2+2, partition.sizeOfLabel(label1));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied1));
	EXPECT_EQ((size_t) 1, partition.sizeOfLabel(2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(3));
	EXPECT_EQ((size_t) 1, partition.sizeOfLabel(4));

	EXPECT_TRUE(partition.isUsed(label1));
	EXPECT_FALSE(partition.isUsed(labelEmptied1));
	EXPECT_TRUE(partition.isUsed(2));
	EXPECT_FALSE(partition.isUsed(3));
	EXPECT_TRUE(partition.isUsed(4));

	EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());
	EXPECT_EQ(nbObs, partition.nbObs());

	//Continue with by a merge involving an already merged label
	size_t label2 = partition.fusion(label1, 4);
	size_t labelEmptied2 = (label2 == label1 ? 4 : label1);
	EXPECT_TRUE(label2==label1 || label2==4);

	EXPECT_EQ((size_t) label2, partition.label(0));
	EXPECT_EQ((size_t) label2, partition.label(1));
	EXPECT_EQ((size_t) label2, partition.label(2));
	EXPECT_EQ((size_t) 2, partition.label(3));
	EXPECT_EQ((size_t) label2, partition.label(4));
	EXPECT_EQ((size_t) label2, partition.label(5));

	EXPECT_EQ(4+3+2, partition.labelWeight(label2));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied1));
	EXPECT_EQ(4, partition.labelWeight(2));
	EXPECT_EQ(0, partition.labelWeight(3));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied2));

	EXPECT_EQ((size_t) 2+2+1, partition.sizeOfLabel(label2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied1));
	EXPECT_EQ((size_t) 1, partition.sizeOfLabel(2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(3));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied2));

	EXPECT_TRUE(partition.isUsed(label2));
	EXPECT_FALSE(partition.isUsed(labelEmptied1));
	EXPECT_TRUE(partition.isUsed(2));
	EXPECT_FALSE(partition.isUsed(3));
	EXPECT_FALSE(partition.isUsed(labelEmptied2));

	EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());
	EXPECT_EQ(nbObs, partition.nbObs());

	//Then, a merge involving empty labels
	size_t label3 = partition.fusion(3, labelEmptied2);
	EXPECT_TRUE( label3==3 || label3==labelEmptied2);

	EXPECT_EQ((size_t) label2, partition.label(0));
	EXPECT_EQ((size_t) label2, partition.label(1));
	EXPECT_EQ((size_t) label2, partition.label(2));
	EXPECT_EQ((size_t) 2, partition.label(3));
	EXPECT_EQ((size_t) label2, partition.label(4));
	EXPECT_EQ((size_t) label2, partition.label(5));

	EXPECT_EQ(4+3+2, partition.labelWeight(label2));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied1));
	EXPECT_EQ(4, partition.labelWeight(2));
	EXPECT_EQ(0, partition.labelWeight(3));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied2));

	EXPECT_EQ((size_t) 2+2+1, partition.sizeOfLabel(label2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied1));
	EXPECT_EQ((size_t) 1, partition.sizeOfLabel(2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(3));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied2));

	EXPECT_TRUE(partition.isUsed(label2));
	EXPECT_FALSE(partition.isUsed(labelEmptied1));
	EXPECT_TRUE(partition.isUsed(2));
	EXPECT_FALSE(partition.isUsed(3));
	EXPECT_FALSE(partition.isUsed(labelEmptied2));

	EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());
	EXPECT_EQ(nbObs, partition.nbObs());

	//Now, try to merge a label and itself
	EXPECT_EQ((size_t) 2, partition.fusion(2, 2));

	EXPECT_EQ((size_t) label2, partition.label(0));
	EXPECT_EQ((size_t) label2, partition.label(1));
	EXPECT_EQ((size_t) label2, partition.label(2));
	EXPECT_EQ((size_t) 2, partition.label(3));
	EXPECT_EQ((size_t) label2, partition.label(4));
	EXPECT_EQ((size_t) label2, partition.label(5));

	EXPECT_EQ(4+3+2, partition.labelWeight(label2));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied1));
	EXPECT_EQ(4, partition.labelWeight(2));
	EXPECT_EQ(0, partition.labelWeight(3));
	EXPECT_EQ(0, partition.labelWeight(labelEmptied2));

	EXPECT_EQ((size_t) 2+2+1, partition.sizeOfLabel(label2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied1));
	EXPECT_EQ((size_t) 1, partition.sizeOfLabel(2));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(3));
	EXPECT_EQ((size_t) 0, partition.sizeOfLabel(labelEmptied2));

	EXPECT_TRUE(partition.isUsed(label2));
	EXPECT_FALSE(partition.isUsed(labelEmptied1));
	EXPECT_TRUE(partition.isUsed(2));
	EXPECT_FALSE(partition.isUsed(3));
	EXPECT_FALSE(partition.isUsed(labelEmptied2));

	EXPECT_EQ(nbMaxLabels, partition.maxNbLabels());
	EXPECT_EQ(nbObs, partition.nbObs());
}

TEST(Partition, weights) {
	size_t nbObs(5);
	size_t nbMaxLabels(4);
	DoubleVector w( { 1.1, 2.3, 3.1, -2.1, 3 });

	Partition partition(nbObs, nbMaxLabels);
	partition.setWeights(w);
	partition.setLabels( { 1, 2, 0, 0, 1 });

	//Check initial state
	for (size_t obs = 0; obs < nbObs; obs++) {
		EXPECT_DOUBLE_EQ(w[obs], partition.obsWeight(obs));
	}

	EXPECT_DOUBLE_EQ(w[2] + w[3], partition.labelWeight(0));
	EXPECT_DOUBLE_EQ(w[0] + w[4], partition.labelWeight(1));
	EXPECT_DOUBLE_EQ(w[1], partition.labelWeight(2));
	EXPECT_DOUBLE_EQ(0, partition.labelWeight(3));

	//Make sure we still have correct weights after moving some observation
	partition.shift(2, 3);
	for (size_t obs = 0; obs < nbObs; obs++) {
		EXPECT_DOUBLE_EQ(w[obs], partition.obsWeight(obs));
	}

	EXPECT_DOUBLE_EQ(w[3], partition.labelWeight(0));
	EXPECT_DOUBLE_EQ(w[0] + w[4], partition.labelWeight(1));
	EXPECT_DOUBLE_EQ(w[1], partition.labelWeight(2));
	EXPECT_DOUBLE_EQ(w[2], partition.labelWeight(3));
}
