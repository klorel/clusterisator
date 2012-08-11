/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMeansAlgo.hpp"

KMeansAlgo::KMeansAlgo(RectMatrix const & input) :
		_input(input), _partition(input.getN()) {

}

KMeansAlgo::~KMeansAlgo() {
}

size_t KMeansAlgo::getK() const {
	return _centers.getN();
}
void KMeansAlgo::set(size_t k) {
	_centers.allocate(_input.getM(), k);
}
void KMeansAlgo::set(Partition const & partition) {
	_partition = partition;
	set(_partition.nbLabels());
	computeCenters();
}

void KMeansAlgo::computeCenters() {
	_centers.assign(0);
	for (size_t i(0); i < _input.getN(); ++i) {
		for (size_t d(0); d < _input.getM(); ++d) {
			_centers.plus(_partition.label(i), d, _input.get(i, d));
		}
	}
}

void KMeansAlgo::random() {
	if (getK() > 0) {
		IndexedList nodes(_partition.nbObs(), true);
		for (size_t k(0); k < getK(); ++k) {
			size_t const n(nodes.pop_random());
			_partition.shift(n, k);
		}
		for (auto const & n : nodes)
			_partition.shift(n, Number::Generator() % getK());
		computeCenters();
	}
}

size_t KMeansAlgo::getNearest(size_t i) const {
	size_t nearest(_partition.label(i));
	Double minDistance(std::numeric_limits<Double>::max());
	for (size_t k(0); k < getK(); ++k) {
		Double const d(getDistance(i, k));
		if (d < minDistance) {
			minDistance = d;
			nearest = k;
		}
	}
	return nearest;
}

Double KMeansAlgo::getDistance(size_t i, size_t k) const {
	Double result(0);
	for (size_t d(0); d < _input.getM(); ++d) {
		Double const size(_partition.sizeOfLabel(k));
		result += std::pow(_input.get(i, d) - _centers.get(k, d) / size, 2);
	}
	return result;
}
void KMeansAlgo::loop(std::vector<std::pair<size_t, size_t> > & moves) {
	moves.clear();
	for (size_t i(0); i < _input.getN(); ++i) {
		size_t const k(getNearest(i));
		if (k != _partition.label(i)) {
			moves.push_back(std::make_pair(i, k));
		}
	}
}

void KMeansAlgo::run(size_t maxIte) {
	size_t ite(0);
	std::vector<std::pair<size_t, size_t> > moves;
	moves.reserve(_partition.nbObs());
	bool stop(false);
	do {
		++ite;
		computeCenters();
		loop(moves);
		if (moves.empty()) {
			stop = true;
		} else {
			for (auto const & move : moves) {
				size_t const i(move.first);
				size_t const from(_partition.label(i));
				size_t const to(move.second);
				// update centroids
				for (size_t d(0); d < _input.getM(); ++d) {
					_centers.plus(from, d, -_input.get(i, d));
					_centers.plus(to, d, _input.get(i, d));
				}
				_partition.shift(from, to);
			}
		}
		/*
		 * compue cost
		 */

	} while (ite != maxIte && !stop);
}
