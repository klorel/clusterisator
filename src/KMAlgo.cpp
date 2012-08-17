/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

KMAlgo::KMAlgo(KMPartition & input) :
		_input(input), _d(input.nbObs(), 0), _cost(0) {
	_ite = 0;
	_old = 0;
	_pertLabels = IndexedList(input.getK());
	_pertObs = IndexedList(input.nbObs());

}

KMAlgo::~KMAlgo() {
}

// we suppose to have
void KMAlgo::computeDistances() {
	_cost = _input.cst();
	_distances.clear();
	//	for (auto const & label : _pertLabels)
	//		Insert(_input.list(label), _pertObs);
	//	for (auto const & obs : _pertObs) {
	for (size_t obs(0); obs < _input.nbObs(); ++obs) {
		//		if (_positions[obs] != _distances.end())
		//			_distances.erase(_positions[obs]);
		if (_input.sizeOfLabel(_input.label(obs)) == 1)
			_d[obs] = Zero<Double>();
		else
			_d[obs] = _input.getDistance(obs) * _input.obsWeight(obs);
		_cost += _d[obs];
		_distances.insert(std::make_pair(_d[obs], obs));
	}
}

void KMAlgo::random() {
	_input.random(_input.getK());
}
void KMAlgo::hMeansLoop(Moves & moves) {
	moves.clear();
	for (size_t i(0); i < _input.nbObs(); ++i) {
		std::pair<size_t, Double> k(getClosest(i));
		if (k.first != _input.label(i)) {
			moves.push_back(std::make_pair(i, k.first));
			//			std::cout << i << " --> " << k.first << " | "
			//					<< _pertLabels.contains(k.first) << "\n";
		}
	}
}

void KMAlgo::singleton() {
	_empty.clear();
	PushBack(_input.unUsed(), _empty);
	//		DisplayContainer(OUT<<"\nused : ", _input.used());
	//		DisplayContainer(OUT<<"empty : ", _empty);
	//		DisplayContainer(OUT<<"unUsed : ", _input.unUsed());
	//	_input.checkLists();
	assert(_empty.size() == _input.unUsed().size());
	assert(_empty.size() + _input.nbLabels() == _input.getK());

	while (!_empty.empty()) {
		assert(!_input.isUsed(_empty.back()));
		assert(_distances.begin()->first>Zero<Double>());
		shift(_distances.begin()->second, _empty.back());
		_cost -= _distances.begin()->first;
		assert(_input.isUsed(_empty.back()));
		_empty.pop_back();

		_distances.erase(_distances.begin());
	}
}
void KMAlgo::hMeans(size_t maxIte) {
	_timer.restart();
	headers(OUT);

	Moves moves;
	moves.reserve(_input.nbObs());

	bool stop(false);
	_ite = 0;

	_pertObs.clear();
	_pertLabels.clear();
	_old = _cost;

	out(OUT);
	_pertLabels.clear();
	for (auto const & label : _input.used())
		_pertLabels.insert(label);
	_pertObs.fill();

	computeDistances();
	_empty.clear();
	do {
		assert(checkCost());
		++_ite;
		hMeansLoop(moves);
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

		assert(_old>=_cost);

	} while (_ite != maxIte && !stop);
	std::cout << computeCost() << "\n";
}

void KMAlgo::kMeans(size_t maxIte) {
	_timer.restart();
	headers(OUT);

	bool improvement(false);

	computeDistances();
	assert(checkCost());
	_ite = 0;
	out(OUT);
	_pertLabels.clear();
	for (auto const & label : _input.used())
		_pertLabels.insert(label);
	_pertObs.fill();
	do {
		_old = _cost;
		++_ite;
		improvement = false;
		for (size_t obs(0); obs < _input.nbObs(); ++obs) {
			std::pair<size_t, Double> delta(getBest(obs));
			if (delta.second < 0) {
				_cost += delta.second;
				shift(obs, delta.first);
				improvement = true;
				//				assert(checkCost());
			}
		}
		out(OUT);
		_pertObs.clear();
		_pertLabels.clear();
	} while (_ite != maxIte && improvement);

}
bool KMAlgo::checkCost() const {
	if (!IsEqual(computeCost(), _cost)) {
		OUT<<std::setprecision(20)<< computeCost();
		OUT<<" != ";
		OUT<<std::setprecision(20)<<_cost<<"\n";
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
	shift(node, to);

}

void KMAlgo::shift(size_t node, size_t to) {
	size_t const from(_input.label(node));
	_pertObs.insert(node);
	_pertLabels.insert(from);
	_pertLabels.insert(to);
	_input.shift(node, to);
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
	out("---------------", "");
	out("nbObs", _input.nbObs());
	out("nbCluster", _input.nbLabels());
	out("---------------", "");
	stream << std::setw(10) << "TIME";
	stream << std::setw(10) << "ITERATION";
	stream << std::setw(20) << "COST";
	stream << std::setw(20) << "DELTA(%)";
	stream << std::setw(15) << "PERT OBS";
	stream << std::setw(15) << "PERT LABELS";
	stream << std::endl;
}

Double KMAlgo::computeCost() const {
	Double result(_input.cst());
	for (size_t i(0); i < _input.nbObs(); ++i) {
		result += _input.obsWeight(i) * _input.getDistance(i);
	}
	return result;
}

Double KMAlgo::getDelta(size_t i, size_t l, size_t j) const {
	assert(_input.label(i)==l);
	if (l != j)
		return _input.obsWeight(i)
				* (_input.getDistance(i, j) * _input.getCoeff<true>(i, j)
						- _input.getDistance(i, l)
								* _input.getCoeff<false>(i, l));
	else
		return Zero<Double>();
}
Double KMAlgo::getDelta(size_t i, size_t j) const {
	size_t const l(_input.label(i));
	return getDelta(i, l, j);
}

std::pair<size_t, Double> KMAlgo::getBest(size_t i) const {
	size_t const l(_input.label(i));
	std::pair<size_t, Double> min(std::make_pair(l, 0));

	if (_input.sizeOfLabel(l) != 1) {
		Double const cst(-_input.getDistance(i) * _input.getCoeff<false>(i, l));
		for (size_t j(0); j < _input.getK(); ++j) {
			if (j != l) {
				Double delta(cst);
				delta += _input.getDistance(i, j) * _input.getCoeff<true>(i, j);
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
std::pair<size_t, Double> KMAlgo::getClosest(size_t i) const {
	size_t const l(_input.label(i));
	std::pair<size_t, Double> min(std::make_pair(l, _d[i]));
	if (_pertLabels.contains(l)) {
		for (size_t k(0); k < _input.getK(); ++k) {
			if (k != l) {
				std::pair<size_t, Double> const d(
						std::make_pair(k, _input.getDistance(i, k)));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	} else {
		for (auto const & k : _pertLabels) {
			//		for (size_t k(0); k < _input.getK(); ++k) {
			if (k != l) {
				std::pair<size_t, Double> const d(
						std::make_pair(k, _input.getDistance(i, k)));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	}
	return min;
}

Double KMAlgo::ComputeMssc(IPartition const & x, KMInstance const & instance) {
	RectMatrix centers(x.maxNbLabels(), instance.nbAtt());
	centers.assign(0);
	DoubleVector weights(x.maxNbLabels(), Zero<Double>());
	for (auto const & l : x.used()) {
		for (auto const & i : x.list(l)) {
			weights[l] += instance.weight(i);
			for (size_t d(0); d < instance.nbAtt(); ++d)
				centers.plus(l, d, instance.get(i, d) * instance.weight(i));
		}
	}
	Double result(Zero<Double>());
	for (size_t i(0); i < instance.nbObs(); ++i) {
		size_t const l(x.label(i));
		for (size_t d(0); d < instance.nbAtt(); ++d)
			result += instance.weight(i)
					* std::pow(
							instance.get(i, d) - centers.get(l, d) / weights[l],
							2);

	}

	return result;
}

KMPartition & KMAlgo::partition() {
	return _input;

}
