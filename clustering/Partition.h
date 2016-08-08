/*
 * Partition.h
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef PARTITION_HPP_
#define PARTITION_HPP_

#include "common.h"
#include "IPartition.h"

class Partition;

void operator>>(std::istream &, Partition &);
void operator>>(std::string const &, Partition &);

/**
 * Implementation of IPartition such that, in particular:
 * - by default, every observation belongs to the label 0
 * - by default, every observation as a weight 1.0
 */
class Partition: public IPartition {
public:
	Partition(int nbObs, int nbMaxLabels = 1);
	Partition(Partition const &);
	virtual ~Partition();

	void setLabels(Partition const &);
	void setLabels(IntVector const & labels);
	bool shift(int observation, int to);
	int fusion(int label1, int label2);

	/**
	 * Reinitialize the partition with the new given number of observations and
	 * the new number of maximum labels
	 *
	 * The weights are reinitialized to 1.0 too, and every observation is put in
	 * the label 0
	 */
	void oneLabel(int nbObs, int nbMaxLabel);

	int nbObs() const;
	int nbLabels() const;
	int maxNbLabels() const;
	void setWeights(DoubleVector const & weights);
	Double obsWeight(int) const;
	Double & obsWeight(int);

	DoubleVector const & labelWeights() const;
	Double labelWeight(int) const;
	Double & labelWeight(int);
	int & sizeOfLabel(int l);
	int sizeOfLabel(int l) const;
	int getUnUsedLabel() const;
	bool allLabelsUsed() const;
	IndexedList const & unUsed() const;
	IndexedList const & usedLabels() const;
	IndexedList const & unUsedLabels() const;
	bool isUsed(int l) const;
	IntList const & observations(int label) const;

	int label(int n) const;
	int & label(int n);
	IntVector const & labels() const;
	Partition & operator=(Partition const & rhs);
	void random();
	void random(int k);

	bool checkLists() const;
	bool checkWeights() const;

private:
	IntVector _labels;
	// la composition de chaque label
	LabelLists _labelLists;
	// la position de chaque noeud dans les listes de composition
	NodePositions _nodePosition;
	//
	DoubleVector _nodeWeights;
	DoubleVector _labelWeights;
	// la taille des labels
	IntVector _size;
	// les labels utilisés
	IndexedList _usedLabels;
	// les labels libres
	IndexedList _unUsedLabels;
};

inline int Partition::nbObs() const {
	return (int) _labels.size();
}
inline int Partition::nbLabels() const {
	return _usedLabels.size();
}

inline int Partition::maxNbLabels() const {
	return (int)_size.size();
}
inline int Partition::sizeOfLabel(int l) const {
	return _size[l];
}

inline int & Partition::sizeOfLabel(int l) {
	return _size[l];
}

inline IndexedList const & Partition::usedLabels() const {
	return _usedLabels;
}
inline IndexedList const & Partition::unUsedLabels() const {
	return _unUsedLabels;
}

inline IntList const & Partition::observations(int label) const {
	return _labelLists[label];
}

inline int Partition::getUnUsedLabel() const {
	return _unUsedLabels.front();
}

inline bool Partition::allLabelsUsed() const {
	return _unUsedLabels.empty();
}

inline bool Partition::isUsed(int l) const {
	return _usedLabels.contains(l);
}

inline IndexedList const & Partition::unUsed() const {
	return _unUsedLabels;
}

inline int Partition::label(int n) const {
	return _labels[n];
}
inline int & Partition::label(int n) {
	return _labels[n];
}
inline IntVector const & Partition::labels() const {
	return _labels;
}

inline Double Partition::obsWeight(int i) const {
	return _nodeWeights[i];
}
inline Double & Partition::obsWeight(int i) {
	return _nodeWeights[i];
}

inline Double Partition::labelWeight(int i) const {
	return _labelWeights[i];
}
inline Double & Partition::labelWeight(int i) {
	return _labelWeights[i];
}

inline DoubleVector const & Partition::labelWeights() const {
	return _labelWeights;
}
#endif /* PARTITION_HPP_ */

