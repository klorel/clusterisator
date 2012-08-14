/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

KMAlgo::KMAlgo(RectMatrix const & input) :
		_input(input), _partition(input.getN()), _d(input.getN(), 0), _cost(0) {
	_ite = 0;
	_nbLabels = 0;
	_old = 0;
}

KMAlgo::~KMAlgo() {
}

size_t KMAlgo::getK() const {
	return _centers.getN();
}
void KMAlgo::set(size_t k) {
	_centers.allocate(k, _input.getM());
}
void KMAlgo::set(Partition const & partition) {
	_partition = partition;
	set(_partition.nbLabels());
	computeCenters(_centers);
	computeCost();
}

Double KMAlgo::size(size_t k) const {
	return static_cast<Double>(_partition.sizeOfLabel(k));
}
// we suppose to have
void KMAlgo::computeCost() {
	_cost = 0;
	for (size_t i(0); i < _input.getN(); ++i) {
		_d[i] = getDistance(i, _partition.label(i));
		_cost += _d[i];
	}
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
	if (getK() > 0 && _partition.nbObs() >= getK()) {
		IndexedList nodes(_partition.nbObs(), true);
		for (size_t k(0); k < getK(); ++k) {
			size_t const n(nodes.pop_random());
			_partition.shift(n, k);
		}
		for (auto const & n : nodes)
			_partition.shift(n, Number::Generator() % getK());
	}
}

Double KMAlgo::getDistance(size_t i, size_t k) const {
	Double result(0);
	for (size_t d(0); d < _input.getM(); ++d) {
		result += std::pow(_input.get(i, d) - _centers.get(k, d) / size(k), 2);
	}
//	return std::sqrt(result);
	return result;
}

KMAlgo::CentroidData KMAlgo::getNearest(size_t i) const {
	CentroidData min(std::make_pair(_partition.label(i), _d[i]));
	for (size_t k(0); k < getK(); ++k) {
		if (k != _partition.label(i)) {
			CentroidData const d(std::make_pair(k, getDistance(i, k)));
			if (d.second < min.second) {
				min = d;
			}
		}
	}
	return min;
}

void KMAlgo::loop(Moves & moves) {
	moves.clear();
	for (size_t i(0); i < _input.getN(); ++i) {
		CentroidData const k(getNearest(i));
		if (k.first != _partition.label(i)) {
			//			OUT<< std::setw(4) << i;
			//			OUT <<std::setw(15)<< k.second - getDistance(i, _partition.label(i));
			//			OUT<<"\n";
			moves.push_back(std::make_pair(i, k.first));
		}
	}
}

void KMAlgo::run(size_t maxIte) {
	_timer.restart();
	headers(OUT);
	size_t ite(0);
	Moves moves;
	moves.reserve(_partition.nbObs());
	bool stop(false);

	computeCenters(_centers);
	computeCost();
	_ite = 0;
	_nbLabels = _partition.nbLabels();
	_pertObs.clear();
	_pertLabels.clear();
	_old = _cost;

	out(OUT);
	do {
//		assert(_partition.nbLabels() == getK());
		++ite;
		loop(moves);
		_old = _cost;
		if (moves.empty()) {
			stop = true;
		} else {
			apply(moves);
		}
		out(OUT);
	} while (ite != maxIte && !stop);
}

void KMAlgo::apply(Moves const & moves) {
	_pertObs.clear();
	_pertLabels.clear();
	if (!moves.empty()) {
		for (auto const & move : moves) {
			apply(move);
		}
		computeCost();
	}
	_nbLabels = _partition.nbLabels();
}
void KMAlgo::apply(Move const & move) {

	size_t const i(move.first);
	size_t const from(_partition.label(i));
	size_t const to(move.second);
	_pertObs.insert(i);
	_pertLabels.insert(from);
	_pertLabels.insert(to);
	// update centroids
	for (size_t d(0); d < _input.getM(); ++d) {
		if (size(from) == 1)
			_centers.get(from, d) = 0;
		else
			_centers.plus(from, d, -_input.get(i, d));
		_centers.plus(to, d, _input.get(i, d));
	}
	_partition.shift(i, to);
}

void KMAlgo::out(std::ostream & stream) const {
	stream << std::setw(10) << _timer.elapsed();
	stream << std::setw(10) << _ite;
	stream << std::setw(10) << _nbLabels;
	stream << std::setw(20) << std::setprecision(10) << _cost;
	if (_old != Zero<Double>())
		stream << std::setw(20) << (_old - _cost) / _old * 100;

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
	stream << std::setw(10) << "NB LABELS";
	stream << std::setw(20) << "COST";
	stream << std::setw(20) << "DELTA(%)";
	stream << std::setw(15) << "PERT OBS";
	stream << std::setw(15) << "PERT LABELS";
	stream << std::endl;
}
void KMAlgo::check(std::string const & text) const {
	// centroids
	//	RectMatrix mat(getK(), _input.getM());
	//	computeCenters(mat);
	//	if (mat != _centers) {
	//		assert(false && "Bug dans les centres");
	//	}

	//	for (size_t i(0); i < _input.getN(); ++i) {
	//		Double const d(getDistance(i, _partition.label(i)));
	//		bool const b(CheckEqual(d, _d[i]));
	//		if (!b) {
	//			OUT<< std::setw(20) << text<<" wrong distances ";
	//			OUT<< std::setw(4)<<i;
	//			OUT<< std::setw(20)<<d;
	//			OUT<< " != ";
	//			OUT<< std::setw(20)<<_d[i];
	//			OUT<< "\n";
	//			assert(false && "Bug dans les distances");
	//		}
	//	}

}
void KMAlgo::newMustLink(size_t i, size_t j) {
	_mustLink.push_back(std::make_pair(i, j));
}
void KMAlgo::newCannotLink(size_t i, size_t j) {
	_cannotLink.push_back(std::make_pair(i, j));
}

void KMAlgo::check(size_t k) const {

}
