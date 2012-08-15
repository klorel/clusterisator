/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

KMAlgo::KMAlgo(RectMatrix const & input, size_t k) :
		_input(input), _centers(k, _input.getM()), _partition(input.getN(), k), _d(
				input.getN(), 0), _cost(0), _mustLink(input.getN()), _cannotLink(
				_input.getN()) {
	_ite = 0;
	_old = 0;
	_pertLabels = IndexedList(k);
	_pertObs = IndexedList(input.getN());

}

KMAlgo::~KMAlgo() {
}

void KMAlgo::set(Partition const & partition) {
	_partition = partition;
	computeCenters();
	computeDistances();
}

// we suppose to have
void KMAlgo::computeDistances() {
	_cost = 0;
	_distances.clear();
	//	for (auto const & label : _pertLabels)
	//		Insert(_partition.list(label), _pertObs);
	//	for (auto const & obs : _pertObs) {
	for (size_t obs(0); obs < _input.getN(); ++obs) {
		//		if (_positions[obs] != _distances.end())
		//			_distances.erase(_positions[obs]);
		if (_partition.sizeOfLabel(_partition.label(obs)) == 1)
			_d[obs] = Zero<Double>();
		else
			_d[obs] = getDistance(obs);
		_cost += _d[obs];
		_distances.insert(std::make_pair(_d[obs], obs));
	}
}
void KMAlgo::computeCenters() {
	computeCenters(_centers);
}
void KMAlgo::computeCenters(RectMatrix & centers) const {
	centers.assign(Zero<Double>());
	for (size_t i(0); i < _input.getN(); ++i) {
		for (size_t d(0); d < _input.getM(); ++d) {
			centers.plus(_partition.label(i), d, _input.get(i, d));
		}
	}
}

void KMAlgo::random() {
	_partition.random(getK());
}
Double KMAlgo::getDelta(size_t i, size_t l, size_t j) const {
	return getDistance(i, j) * getCoeff<true>(j)
			- getDistance(i, l) * getCoeff<false>(l);
}
KMAlgo::CentroidData KMAlgo::getBest(size_t i) const {
	size_t const l(_partition.label(i));

	CentroidData min(std::make_pair(l, 0));
	if (_partition.sizeOfLabel(l) != 1) {
		Double const cst(-getDistance(i) * getCoeff<false>(l));
		for (size_t j(0); j < getK(); ++j) {
			if (j != l) {
				Double delta(cst);
				delta += getDistance(i, j) * getCoeff<true>(j);
				assert(IsEqual(delta, getDelta(i,l,j)));
				if (delta < min.second) {
					min.first = j;
					min.second = delta;
				}
			}
		}
	}

	return min;
}
KMAlgo::CentroidData KMAlgo::getClosest(size_t i) const {
	CentroidData min(std::make_pair(_partition.label(i), _d[i]));
	if (_pertLabels.contains(_partition.label(i))) {
		for (size_t k(0); k < getK(); ++k) {
			if (k != _partition.label(i)) {
				CentroidData const d(std::make_pair(k, getDistance(i, k)));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	} else {
		for (auto const & k : _pertLabels) {
			//		for (size_t k(0); k < getK(); ++k) {
			if (k != _partition.label(i)) {
				CentroidData const d(std::make_pair(k, getDistance(i, k)));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	}
	return min;
}

void KMAlgo::loop(Moves & moves) {
	moves.clear();
	for (size_t i(0); i < _input.getN(); ++i) {
		CentroidData const k(getClosest(i));
		if (k.first != _partition.label(i)) {
			moves.push_back(std::make_pair(i, k.first));
			//			std::cout << i << " --> " << k.first << " | "
			//					<< _pertLabels.contains(k.first) << "\n";
		}
	}
}

void KMAlgo::singleton() {

	_empty.clear();
	PushBack(_partition.unUsed(), _empty);
//		DisplayContainer(OUT<<"\nused : ", _partition.used());
//		DisplayContainer(OUT<<"empty : ", _empty);
//		DisplayContainer(OUT<<"unUsed : ", _partition.unUsed());
//	_partition.checkLists();
	assert(_empty.size() == _partition.unUsed().size());
	assert(_empty.size() + _partition.nbLabels() == getK());

	while (!_empty.empty()) {
		assert(!_partition.isUsed(_empty.back()));
		assert(_distances.begin()->first>Zero<Double>());
		move(_distances.begin()->second, _empty.back());
		_cost -= _distances.begin()->first;
		assert(_partition.isUsed(_empty.back()));
		_empty.pop_back();

		_distances.erase(_distances.begin());
	}
}
void KMAlgo::run(size_t maxIte) {
	_timer.restart();
	headers(OUT);

	Moves moves;
	moves.reserve(_partition.nbObs());

	bool stop(false);
	_ite = 0;

	_pertObs.clear();
	_pertLabels.clear();
	_old = _cost;

	out(OUT);
	_pertLabels.clear();
	for (auto const & label : _partition.used())
		_pertLabels.insert(label);
	_pertObs.fill();

	computeCenters();
	computeDistances();
	_empty.clear();
	do {
		assert(checkCost());
		++_ite;
		loop(moves);
		_old = _cost;
		if (moves.empty()) {
			stop = true;
		} else {
			apply(moves);
		}
		computeDistances();

		singleton();
		computeDistances();

		assert(checkCost());
		out(OUT);

	} while (_ite != maxIte && !stop);
	std::cout << computeCost() << "\n";
}

void KMAlgo::run2() {
	_timer.restart();
	headers(OUT);

	bool improvement(false);
	computeCenters();
	computeDistances();
	assert(checkCost());
	_ite = 0;
	out(OUT);
	do {
		_old = _cost;
		++_ite;
		improvement = false;
		_pertObs.clear();
		_pertLabels.clear();
		for (size_t obs(0); obs < _partition.nbObs(); ++obs) {
			CentroidData delta(getBest(obs));
			if (delta.second < 0) {
				_cost += delta.second;
				move(obs, delta.first);
				improvement = true;
				assert(checkCost());
			}
		}
		out(OUT);
	} while (improvement);

}
bool KMAlgo::checkCost() const {
	if (!IsEqual(computeCost(), _cost)) {
		OUT<<computeCost()<<" != "<<_cost<<"\n";
		return false;
	}
	return true;
}
void KMAlgo::apply(Moves const & moves) {
	_pertObs.clear();
	_pertLabels.clear();
	if (!moves.empty()) {
		for (auto const & move : moves) {
			apply(move);
		}
	}
}
void KMAlgo::apply(Move const & m) {
	size_t const node(m.first);
	size_t const to(m.second);
	move(node, to);

}

void KMAlgo::move(size_t node, size_t to) {
	size_t const from(_partition.label(node));

	_pertObs.insert(node);
	_pertLabels.insert(from);
	_pertLabels.insert(to);

	// update centroids
	for (size_t d(0); d < _input.getM(); ++d) {
		if (size(from) == 1)
			_centers.get(from, d) = 0;
		else
			_centers.plus(from, d, -_input.get(node, d));
		_centers.plus(to, d, _input.get(node, d));
	}

	_partition.shift(node, to);

}

void KMAlgo::out(std::ostream & stream) const {
	stream << std::setw(10) << _timer.elapsed();
	stream << std::setw(10) << _ite;
	stream << std::setw(20) << std::setprecision(10) << _cost;

	stream << std::setw(20)
			<< (IsEqual(_cost, _old) ? 0 : (_old - _cost) / _old * 100);

	stream << std::setw(15) << _pertObs.size();
	stream << std::setw(15) << _pertLabels.size();
	stream << std::endl;
}

void KMAlgo::headers(std::ostream & stream) const {
	out(OUT, "---------------", "");
	out(OUT, "nbObs", _partition.nbObs());
	out(OUT, "nbCluster", _partition.nbLabels());
	out(OUT, "---------------", "");
	stream << std::setw(10) << "TIME";
	stream << std::setw(10) << "ITERATION";
	stream << std::setw(20) << "COST";
	stream << std::setw(20) << "DELTA(%)";
	stream << std::setw(15) << "PERT OBS";
	stream << std::setw(15) << "PERT LABELS";
	stream << std::endl;
}
void KMAlgo::newMustLink(size_t i, size_t j) {
	_mustLink.newCtr(i, j);
}
void KMAlgo::newCannotLink(size_t i, size_t j) {
	_cannotLink.newCtr(i, j);
}

Double KMAlgo::computeCost() const {
	Double result(0);
	for (size_t i(0); i < _input.getN(); ++i)
		result += getDistance(i);
	return result;
}

void KMAlgo::testDelta() {
	size_t const obs(0);
	size_t const l(_partition.label(obs));
	size_t const j(37);
	computeCenters();
	Double oldCost(computeCost());
	Double deltaCost(getDelta(obs, l, j));
	move(obs, j);
	computeCenters();
	Double newCost(computeCost());
	std::cout << std::setprecision(15) << oldCost << std::endl;
	std::cout << std::setprecision(15) << oldCost + deltaCost << std::endl;
	std::cout << std::setprecision(15) << newCost << std::endl;
	std::cout << std::setprecision(15) << deltaCost << std::endl;
	std::cout << std::setprecision(15) << newCost - oldCost << std::endl;
}
