/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

#define LOCAL_DEBUG
std::string const KMAlgo::HMEANS = "H-MEANS";
std::string const KMAlgo::KMEANS = "K-MEANS";
KMAlgo::KMAlgo(KMPartition & input) :
		_input(input), _d(input.nbObs(), 0), _cost(0) {
	_ite = 0;
	_old = 0;

	_pertLabels = IndexedList(input.getK());
	_pertNodes = IndexedList(input.nbObs());
	_buffer.reserve(input.nbObs());

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
			_d[obs] = _input.getDistance(obs);
		_cost += _d[obs] * _input.obsWeight(obs);
		_distances.insert(std::make_pair(_d[obs], obs));
	}
}

void KMAlgo::hMeansLoop(Moves & moves) {
	moves.clear();
	for (size_t i(0); i < _input.nbObs(); ++i) {
		std::pair<size_t, Double> const k(getClosest(i));
		if (k.first != _input.label(i)) {
			moves.push_back(std::make_pair(i, k.first));
			assert(getDelta(i,k.first)<0);
			//#ifdef LOCAL_DEBUG
			//			OUT<< i << " : "<<_input.label(i) << " --> " << k.first<<" : "<<getDelta(i,k.first)<<"\n";
			//			OUT<< _d[i]<<" | "<<k.second<<"\n";
			//			OUT<<"\n";
			//			checkDelta(i, k.first);
			//			checkCenters();
			//			OUT<<_input.centers();
			//			assert(getDelta(i,k.first)<0);
			//
			//			break;
			//#endif
			//					<< _pertLabels.contains(k.first) << "\n";
		}
	}
}
void KMAlgo::checkDelta(size_t i, size_t j) {
	size_t const l(_input.label(i));
	Double delta(getDelta(i, j));
	Double cost(computeCost());
	shift(i, j);
	assert(IsEqual(cost+delta,computeCost()));
	shift(i, l);

}
void KMAlgo::checkCenters() const {
	RectMatrix centers(_input.centers());
	_input.computeCenters(centers);
	assert(centers==_input.centers());
}
void KMAlgo::singleton() {
	_buffer.clear();
	PushBack(_input.unUsed(), _buffer);
	//		DisplayContainer(OUT<<"\nused : ", _input.used());
	//		DisplayContainer(OUT<<"empty : ", _empty);
	//		DisplayContainer(OUT<<"unUsed : ", _input.unUsed());
	//	_input.checkLists();
	assert(_buffer.size() == _input.unUsed().size());
	assert(_buffer.size() + _input.nbLabels() == _input.getK());

	while (!_buffer.empty()) {
		assert(!_input.isUsed(_buffer.back()));
		assert(_distances.begin()->first>Zero<Double>());
		shift(_distances.begin()->second, _buffer.back());
		_cost -= _distances.begin()->first;
		assert(_input.isUsed(_buffer.back()));
		_buffer.pop_back();

		_distances.erase(_distances.begin());
	}
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
	_pertLabels.clear();
	_pertNodes.clear();
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
	if (feasible(node, to)) {
		size_t const from(_input.label(node));
		_pertLabels.insert(from);
		_pertLabels.insert(to);
		_pertNodes.insert(node);
		_input.shift(node, to);
	}
}

void KMAlgo::out(std::ostream & stream) const {
	stream << std::setw(10) << _name;
	stream << std::setw(10) << _timer.elapsed();
	stream << std::setw(10) << _ite;
//	stream << std::setw(10) << _input.nbLabels();
	stream << std::setw(20) << std::setprecision(10) << _cost;

	stream << std::setw(20)
			<< (IsEqual(_cost, _old) ? 0 : (_old - _cost) / _old * 100);
	stream << std::setw(15) << _pertNodes.size();
	stream << std::setw(15) << _pertLabels.size();

	stream << std::endl;
}

void KMAlgo::headers(std::ostream & stream) {
	out("---------------", "");
	out("nbObs", _input.nbObs());
	out("nbCluster", _input.nbLabels());
	out("---------------", "");
	stream << std::setw(10) << "ALGO";
	stream << std::setw(10) << "TIME";
	stream << std::setw(10) << "ITERATION";
//	stream << std::setw(10) << "NB LABELS";
	stream << std::setw(20) << "COST";
	stream << std::setw(20) << "DELTA(%)";
	stream << std::setw(15) << "PERT OBS";
	stream << std::setw(15) << "PERT LABELS";
	stream << std::endl;
	_timer.restart();
	_ite = 0;
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
	std::pair<size_t, Double> min(l, Zero<Double>());

	if (_input.sizeOfLabel(l) != 1) {
		Double const cst(-_input.getDistance(i) * _input.getCoeff<false>(i, l));
		for (size_t j(0); j < _input.getK(); ++j) {
			if (j != l && feasible(i, j)) {
				Double delta(cst);
				delta += _input.getDistance(i, j) * _input.getCoeff<true>(i, j);
				delta *= _input.obsWeight(i);
				if (!IsEqual(delta, getDelta(i, l, j))) {

					Double const bigger(
							std::max(std::abs(delta),
									std::abs(getDelta(i, l, j))));
					OUT<<std::setprecision(15)<<(delta - getDelta(i,l,j))/bigger<<"\n";
					OUT<<std::setprecision(15)<<delta<<"\n";
					OUT<<std::setprecision(15)<< getDelta(i,l,j)<<"\n";
					assert(IsEqual(delta, getDelta(i, l, j)));
				}
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
	std::pair<size_t, Double> min(l, _d[i]);
	if (_pertLabels.contains(l)) {
		for (size_t k(0); k < _input.getK(); ++k) {
			if (k != l) {
				std::pair<size_t, Double> const d(k, _input.getDistance(i, k));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	} else {
		for (auto const & k : _pertLabels) {
			//		for (size_t k(0); k < _input.getK(); ++k) {
			if (k != l) {
				std::pair<size_t, Double> const d(k, _input.getDistance(i, k));
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

void KMAlgo::hMeans(size_t maxIte) {
	_name = HMEANS;

	Moves moves;
	moves.reserve(_input.nbObs());

	bool stop(false);
	_pertLabels.clear();
	_pertNodes.clear();
	computeDistances();
	_old = _cost;

	for (auto const & label : _input.used())
		_pertLabels.insert(label);
	_pertNodes.fill();
	_buffer.clear();
	do {
		assert(checkCost());
		++_ite;
		hMeansLoop(moves);
		_old = _cost;
		if (moves.empty()) {
			stop = true;
		} else {
			apply(moves);
			if (_pertLabels.empty())
				stop = true;
		}
		computeDistances();
//		singleton();
//		computeDistances();

		assert(checkCost());
		if (!stop)
			out(OUT);

		assert(_old>=_cost);

	} while (_ite != maxIte && !stop);
//	std::cout << computeCost() << "\n";
}

void KMAlgo::kMeans(size_t maxIte) {
	_name = KMEANS;
//	_timer.restart();
//	headers(OUT);

	bool improvement(false);

	computeDistances();
	assert(checkCost());
//	out(OUT);
	_pertNodes.fill();
	_pertLabels.clear();
	for (auto const & label : _input.used())
		_pertLabels.insert(label);
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
		if (_ite != maxIte && improvement)
			out(OUT);
		_pertLabels.clear();
		_pertNodes.clear();

	} while (_ite != maxIte && improvement);

}
bool KMAlgo::feasible(size_t i, size_t j) const {
	for (auto const & n : _input.mustLinks(i)) {
		if (_input.label(n) != j)
			return false;
	}
	for (auto const & n : _input.cannotLinks(i)) {
		if (_input.label(n) == j)
			return false;
	}
	return true;
}

#undef LOCAL_DEBUG
