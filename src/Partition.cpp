/*
 * Partition.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Partition.hpp"

Partition::Partition(size_t n, size_t k) :
		IPartition(n, k) {
	allocate(n, k);
}
Partition::~Partition() {

}

void Partition::allocate(size_t n, size_t k) {
	_labelLists.assign(k, IntList());
	_size.assign(k, 0);
	_usedLabels.reset(k);
	_unUsedLabels.reset(k);

	_nodePosition.assign(n, _labelLists[0].end());

	_labels.assign(n, 0);

	_usedLabels.clear();
	_usedLabels.insert(0);

	_unUsedLabels.fill();
	_unUsedLabels.erase(0);

	for (size_t n(0); n < nbObs(); ++n) {
		_labelLists[0].push_front(n);
		_nodePosition[n] = _labelLists[0].begin();
	}
	_size[0] = n;
}

void Partition::set(IntVector const & v) {
	for (size_t i(0); i < v.size(); ++i) {
		assert(
				v[i]<maxNbLabels() && "you must provide labels l : 0<=l<maxNbLabels()");
		shift(i, v[i]);
	}

}
void Partition::shift(size_t n, size_t l) {
	if (label(n) != l) {
		--sizeOfLabel(label(n));
		_labelLists[label(n)].erase(_nodePosition[n]);
		if (sizeOfLabel(label(n)) == 0) {
			_usedLabels.erase(label(n));
			_unUsedLabels.insert(label(n));
		}
		if (sizeOfLabel(l) == 0) {
			_unUsedLabels.erase(l);
			_usedLabels.insert(l);
		}
		++sizeOfLabel(l);
		_labelLists[l].push_front(n);
		_nodePosition[n] = _labelLists[l].begin();
		label(n) = l;
	}
}
size_t Partition::fusion(size_t const & label1, size_t const & label2) {
	return -1;
}
bool Partition::checkLists() const {
	for (auto const & l : usedLabel()) {
		//		TRACE_N(*l);
		if (sizeOfLabel(l) == 0)
			OUT<< "error on size of label " << l << "\n";
			for (auto const & n : list(l)) {
				if (n != *_nodePosition[n]) {
					OUT << n << " iterator in label " << l << " is wrong\n";
				}
			}
		}
		return true;
	}

Partition & Partition::operator=(Partition const & rhs) {
	if (this != &rhs) {
		if (rhs.nbObs() != nbObs() || rhs.nbLabels() != rhs.nbLabels())
			allocate(rhs.nbObs(), rhs.nbLabels());
		for (size_t n(0); n < nbObs(); ++n)
			shift(n, rhs.label(n));
	}
	return *this;

}
void Partition::random(size_t k) {
	allocate(nbObs(), k);
	IndexedList nodes(nbObs(), true);
	for (size_t i(0); i < maxNbLabels(); ++i) {
		size_t const n(nodes.pop_random());
		shift(n, i);
	}
	for (auto const & n : nodes)
		shift(n, Number::Generator() % maxNbLabels());

}
