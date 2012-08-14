/*
 * Partition.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef PARTITION_HPP_
#define PARTITION_HPP_

#include "../utils/common.h"

#include "../interfaces/IPartition.hpp"

class Partition: public IPartition {
public:
	Partition(size_t n = 0);
	Partition(IntVector const &);
	virtual ~Partition();
public:
	// déplacement d'un noeud
	void shift(size_t n, size_t p);
	// fusion of two label, return the used label
	size_t fusion(size_t const & label1, size_t const & label2);
public:
	void contigusLabels();
	void set(IntVector const &);
	// le nombre de noeuds
	size_t nbObs() const;
	void setNbObs(size_t);
	// le nombre de labels utilisé dans la solution courante
	size_t nbLabels() const;
	// le nombre de noeud ayant la label l
	size_t & sizeOfLabel(size_t l);
	size_t sizeOfLabel(size_t l) const;
	// récupère un label vide
	size_t getUnUsedLabel() const;
	// est-ce que tout les labels sont utilisés
	bool allLabelsUsed() const;
	// les labels libres
	IndexedList & unUsed();
	IndexedList const & unUsed() const;
	// les labels utilisés
	IndexedList & used();
	IndexedList const & used() const;
	// est-ce que le label est utilisé
	bool isUsed(size_t l) const;
public:
	IndexedList const & usedLabel() const;
	IndexedList const & unUsedLabel() const;

	IntList & list(size_t label);
	IntList const & list(size_t label) const;

	size_t label(size_t n) const;
	size_t & label(size_t n);
public:

	bool checkLists() const;
private:
	IntVector _labels;
	// la composition de chaque label
	LabelLists _labelLists;
	// la position de chaque noeud dans les listes de composition
	NodePositions _nodePosition;
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

inline size_t Partition::sizeOfLabel(size_t l) const {
	return _size[l];
}

inline size_t & Partition::sizeOfLabel(size_t l) {
	return _size[l];
}

inline IndexedList const & Partition::usedLabel() const {
	return _usedLabels;
}
inline IndexedList const & Partition::unUsedLabel() const {
	return _unUsedLabels;
}

inline IntList & Partition::list(size_t label) {
	return _labelLists[label];
}
inline IntList const & Partition::list(size_t label) const {
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

inline IndexedList & Partition::unUsed() {
	return _unUsedLabels;
}

inline IndexedList const & Partition::unUsed() const {
	return _unUsedLabels;
}

inline IndexedList & Partition::used() {
	return _usedLabels;
}

inline IndexedList const & Partition::used() const {
	return _usedLabels;
}
inline size_t Partition::label(size_t n) const {
	return _labels[n];
}
inline size_t & Partition::label(size_t n) {
	return _labels[n];
}

#endif /* PARTITION_HPP_ */
