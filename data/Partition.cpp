/*
 * Partition.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Partition.hpp"

Partition::Partition(size_t n) {
	IntVector bigOne(n, 0);
	set(bigOne);
	//	startWith(IntVector(nbNodes(), 0));
}
Partition::Partition(IntVector const & rhs) {
	set(rhs);
}
Partition::~Partition() {

}
void Partition::contigusLabels() {
	IntVector labels(nbObs(), -1);
	size_t i(0);
	for (auto const & l : used()) {
		for (auto const & n : list(l))
			labels[n] = i;
		++i;
	}
	set(labels);
}
void Partition::set(std::vector<size_t> const & v) {
	setNbObs(v.size());
	_size.assign(nbObs(), 0);
	_usedLabels.clear();
	_unUsedLabels.fill();
	_labelLists.assign(nbObs(), IntList());
	for (size_t n(0); n < nbObs(); ++n) {
		size_t const l(v[n]);
		_labels[n] = l;
		_usedLabels.insert(l);
		_unUsedLabels.erase(l);
		_labelLists[l].push_front(n);
		_nodePosition[n] = _labelLists[l].begin();
		++_size[l];
	}
}
void Partition::shift(size_t n, size_t l) {
	if (label(n) != l) {
		--sizeOfLabel(label(n));
		_labelLists[label(n)].erase(_nodePosition[n]);
		if (sizeOfLabel(label(n)) == 0) {
			_usedLabels.erase(label(n));
			_unUsedLabels.insert(label(n));
			//			OUT << "Result : " << std::endl;
			//			for (IndexedList::const_iterator lIte(begin()); lIte != end(); ++lIte) {
			//				OUT << std::setw(4) << *lIte;
			//				OUT << std::setw(4) << sizeOfLabel(*lIte);
			//				OUT << std::setw(4) << isUsed(*lIte);
			//				OUT << std::endl;
			//			}
			//			OUT << "-----------------" << std::endl;
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
void Partition::setNbObs(size_t n) {
	_labels.resize(n, n);
	_labelLists.resize(n);
	_nodePosition.resize(n);
	_size.resize(n, 0);
	_usedLabels.reset(n);
	_unUsedLabels.reset(n);
}
size_t Partition::nbObs() const {
	return _labels.size();
}
size_t Partition::nbLabels() const {
	return _usedLabels.size();
}

size_t Partition::sizeOfLabel(size_t l) const {
	return _size[l];
}

size_t & Partition::sizeOfLabel(size_t l) {
	return _size[l];
}

IndexedList const & Partition::usedLabel() const {
	return _usedLabels;
}
IndexedList const & Partition::unUsedLabel() const {
	return _unUsedLabels;
}

IntList & Partition::list(size_t label) {
	return _labelLists[label];
}
IntList const & Partition::list(size_t label) const {
	return _labelLists[label];
}

size_t Partition::getUnUsedLabel() const {
	return _unUsedLabels.front();
}

bool Partition::allLabelsUsed() const {
	return _unUsedLabels.empty();
}

bool Partition::isUsed(size_t l) const {
	return _usedLabels.contains(l);
}

IndexedList & Partition::unUsed() {
	return _unUsedLabels;
}

IndexedList const & Partition::unUsed() const {
	return _unUsedLabels;
}

IndexedList & Partition::used() {
	return _usedLabels;
}

IndexedList const & Partition::used() const {
	return _usedLabels;
}
size_t Partition::label(size_t n) const {
	return _labels[n];
}
size_t & Partition::label(size_t n) {
	return _labels[n];
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
