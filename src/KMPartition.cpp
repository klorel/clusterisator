/*
 * KMPartition.cpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#include "src/KMPartition.hpp"

KMPartition::KMPartition(KMInstance const & input, size_t k) :
		Partition(input.nbObs(), k), _input(input), _centers(k, input.nbAtt()) {
	setWeights(_input.weights());
	computeCenters();
	_d.assign(nbObs(), 0);
}
KMPartition::KMPartition(KMInstance const &input, Partition const &rhs) :
		Partition(rhs), _input(input), _centers(rhs.maxNbLabels(),
				input.nbAtt()) {
	setWeights(_input.weights());
	computeCenters();
}
KMPartition::~KMPartition() {

}

// we suppose to have
void KMPartition::computeDistances() {
	_cost = _input.cst();
	_distances.clear();
	//	for (auto const & label : _pertLabels)
	//		Insert(_input.list(label), _pertObs);
	//	for (auto const & obs : _pertObs) {
	for (size_t obs(0); obs < _input.nbObs(); ++obs) {
		//		if (_positions[obs] != _distances.end())
		//			_distances.erase(_positions[obs]);
		if (sizeOfLabel(label(obs)) == 1)
			_d[obs] = 0;
		else
			_d[obs] = getDistance(obs);
		_cost += _d[obs] * obsWeight(obs);
		_distances.insert(std::make_pair(_d[obs], obs));
	}
}
void KMPartition::computeCenters() {
	computeCenters(_centers);
}

void KMPartition::computeCenters(RectMatrix & centers) const {
	centers.allocate(maxNbLabels(), _input.nbAtt());
	for (size_t i(0); i < _input.nbObs(); ++i) {
		for (size_t d(0); d < _input.nbAtt(); ++d) {
			centers.plus(label(i), d, _input.get(i, d) * obsWeight(i));
		}
		//		std::cout << "center " << i << " : ";
		//
		//		for (size_t d(0); d < _input.nbAtt(); ++d)
		//			std::cout << centers.get(i, d) << " ";
		//		std::cout << "\n";
	}
}
bool KMPartition::shift(size_t obs, size_t to) {
	if (feasible(obs, to)) {
		size_t const from(label(obs));
		// recomputing centers is needed
		if (_centers.getN() != maxNbLabels()) {
			computeCenters();
		} else {
			for (size_t d(0); d < _input.nbAtt(); ++d) {
				_centers.plus(from, d, -_input.get(obs, d) * obsWeight(obs));
				_centers.plus(to, d, _input.get(obs, d) * obsWeight(obs));
			}
		}
		Partition::shift(obs, to);
		return true;
	} else
		return false;
}

void KMPartition::shiftForced(size_t obs, size_t to) {
		size_t const from(label(obs));
		// recomputing centers is needed
		if (_centers.getN() != maxNbLabels()) {
			computeCenters();
		} else {
			for (size_t d(0); d < _input.nbAtt(); ++d) {
				_centers.plus(from, d, -_input.get(obs, d) * obsWeight(obs));
				_centers.plus(to, d, _input.get(obs, d) * obsWeight(obs));
			}
		}
		Partition::shift(obs, to);
}

Double KMPartition::getDelta(size_t i, size_t l, size_t j) const {
	assert(label(i)==l);
	if (l != j)
		return obsWeight(i)
				* (getDistance(i, j) * getCoeff<true>(i, j)
						- getDistance(i, l) * getCoeff<false>(i, l));
	else
		return 0;
}
Double KMPartition::getDelta(size_t i, size_t j) const {
	size_t const l(label(i));
	return getDelta(i, l, j);
}


bool KMPartition::feasible(size_t i, size_t j) const {
	for (auto const & n : mustLinks(i)) {
		if (label(n) != j)
			return false;
	}
	for (auto const & n : cannotLinks(i)) {
		if (label(n) == j)
			return false;
	}
	return true;
}

Double KMPartition::cost() const {
	return _cost;
}
Double & KMPartition::cost() {
	return _cost;
}
Double KMPartition::computeCost() const {
	Double result(_input.cst());
	for (size_t i(0); i < nbObs(); ++i) {
		result += obsWeight(i) * getDistance(i);
	}
	return result;
}

void KMPartition::checkDelta(size_t i, size_t j) {
	size_t const l(label(i));
	Double delta(getDelta(i, j));
	Double cost(computeCost());
	shift(i, j);
	assert(IsEqual(cost+delta,computeCost()));
	shift(i, l);

}

void KMPartition::checkCenters() const {
	RectMatrix temp(_centers);
	computeCenters(temp);
	assert(temp==_centers);
}
bool KMPartition::checkCost() const {
	if (!IsEqual(computeCost(), _cost)) {
		OUT<<std::setprecision(20)<< computeCost();
		OUT<<" != ";
		OUT<<std::setprecision(20)<<_cost<<"\n";
		return false;
	}
	return true;
}

bool KMPartition::shift(Move const & m) {
	size_t const node(m.first);
	size_t const to(m.second);
	return shift(node, to);
}
