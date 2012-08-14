/*
 * KMConstraints.cpp
 *
 *  Created on: 14 août 2012
 *      Author: manuel
 */

#include "KMConstraints.hpp"

KMConstraints::KMConstraints(size_t n) :
		_all(), _byObs(n) {

}

KMConstraints::~KMConstraints() {
}

void KMConstraints::newCtr(size_t i, size_t j) {
	_all.insert(std::make_pair(std::min(i, j), std::max(i, j)));
	_byObs[i].insert(j);
	_byObs[j].insert(i);
}

IntSet const & KMConstraints::get(size_t obs) const {
	return _byObs[obs];
}
void KMConstraints::clear() {
	_all.clear();
	for (auto & s : _byObs)
		s.clear();
}
