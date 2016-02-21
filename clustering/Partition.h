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
	Partition(size_t nbObs, size_t nbMaxLabels = 1);
	Partition(Partition const &);
	virtual ~Partition();

	void setLabels(Partition const &);
	void setLabels(IntVector const & labels);
	bool shift(size_t observation, size_t to);
	size_t fusion(size_t const & label1, size_t const & label2);

	/**
	 * Reinitialize the partition with the new given number of observations and
	 * the new number of maximum labels
	 *
	 * The weights are reinitialized to 1.0 too, and every observation is put in
	 * the label 0
	 */
	void oneLabel(size_t nbObs, size_t nbMaxLabel);

	size_t nbObs() const;
	size_t nbLabels() const;
	size_t maxNbLabels() const;
	void setWeights(DoubleVector const & weights);
	Double obsWeight(size_t) const;
	Double & obsWeight(size_t);

	DoubleVector const & labelWeights() const;
	Double labelWeight(size_t) const;
	Double & labelWeight(size_t);
	size_t & sizeOfLabel(size_t l);
	size_t sizeOfLabel(size_t l) const;
	size_t getUnUsedLabel() const;
	bool allLabelsUsed() const;
	IndexedList const & unUsed() const;
	IndexedList const & usedLabels() const;
	IndexedList const & unUsedLabels() const;
	bool isUsed(size_t l) const;
	IntList const & observations(size_t label) const;

	size_t label(size_t n) const;
	size_t & label(size_t n);
	IntVector const & labels() const;
	Partition & operator=(Partition const & rhs);
	void random();
	void random(size_t k);

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

inline size_t Partition::nbObs() const {
	return _labels.size();
}
inline size_t Partition::nbLabels() const {
	return _usedLabels.size();
}

inline size_t Partition::maxNbLabels() const {
	return _size.size();
}
inline size_t Partition::sizeOfLabel(size_t l) const {
	return _size[l];
}

inline size_t & Partition::sizeOfLabel(size_t l) {
	return _size[l];
}

inline IndexedList const & Partition::usedLabels() const {
	return _usedLabels;
}
inline IndexedList const & Partition::unUsedLabels() const {
	return _unUsedLabels;
}

inline IntList const & Partition::observations(size_t label) const {
	return _labelLists[label];
}

inline size_t Partition::getUnUsedLabel() const {
	return _unUsedLabels.front();
}

inline bool Partition::allLabelsUsed() const {
	return _unUsedLabels.empty();
}

inline bool Partition::isUsed(size_t l) const {
	return _usedLabels.contains(l);
}

inline IndexedList const & Partition::unUsed() const {
	return _unUsedLabels;
}

inline size_t Partition::label(size_t n) const {
	return _labels[n];
}
inline size_t & Partition::label(size_t n) {
	return _labels[n];
}
inline IntVector const & Partition::labels() const {
	return _labels;
}

inline Double Partition::obsWeight(size_t i) const {
	return _nodeWeights[i];
}
inline Double & Partition::obsWeight(size_t i) {
	return _nodeWeights[i];
}

inline Double Partition::labelWeight(size_t i) const {
	return _labelWeights[i];
}
inline Double & Partition::labelWeight(size_t i) {
	return _labelWeights[i];
}

inline DoubleVector const & Partition::labelWeights() const {
	return _labelWeights;
}
#endif /* PARTITION_HPP_ */

