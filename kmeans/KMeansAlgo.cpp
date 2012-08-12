/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMeansAlgo.hpp"

KMeansAlgo::KMeansAlgo(RectMatrix const & input) :
		_input(input), _partition(input.getN()), _cost(0) {

}

KMeansAlgo::~KMeansAlgo() {
}

size_t KMeansAlgo::getK() const {
	return _centers.getN();
}
void KMeansAlgo::set(size_t k) {
	_centers.allocate(k, _input.getM());
}
void KMeansAlgo::set(Partition const & partition) {
	_partition = partition;
	set(_partition.nbLabels());
	computeCenters(_centers);
	computeCost();
}

Double KMeansAlgo::size(size_t k) const {
	return static_cast<Double>(_partition.sizeOfLabel(k));
}
// we suppose to have
void KMeansAlgo::computeCost() {
	_cost = 0;
	for (size_t i(0); i < _input.getN(); ++i) {
		size_t const l(_partition.label(i));
		//		_d[i] = getDistance(i, l);
		//		_cost += _d[i];
		_cost += getDistance(i, l);

	}
}
void KMeansAlgo::computeCenters(RectMatrix & centers) const {
	centers.assign(0);
	for (size_t i(0); i < _input.getN(); ++i) {
		for (size_t d(0); d < _input.getM(); ++d) {
			centers.plus(_partition.label(i), d, _input.get(i, d));
		}
	}
	for (size_t k(0); k < getK(); ++k)
		for (size_t d(0); d < _input.getM(); ++d)
			centers.get(k, d) /= size(k);
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
		computeCenters(_centers);
		computeCost();
	}
}

Double KMeansAlgo::getDistance(size_t i, size_t k) const {
	Double result(0);
	for (size_t d(0); d < _input.getM(); ++d) {
		result += std::pow(_input.get(i, d) - _centers.get(k, d), 2);
	}
	return std::sqrt(result);
}

KMeansAlgo::CentroidData KMeansAlgo::getNearest(size_t i) const {
	CentroidData min(
			std::make_pair(_partition.label(i),
					getDistance(i, _partition.label(i))));
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

void KMeansAlgo::loop(Moves & moves) {
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

void KMeansAlgo::run(size_t maxIte) {
	size_t ite(0);
	Moves moves;
	moves.reserve(_partition.nbObs());
	bool stop(false);

	computeCenters(_centers);
	computeCost();
	do {
//		assert(_partition.nbLabels() == getK());
		++ite;
		loop(moves);
		if (moves.empty()) {
			stop = true;
		} else {
			Double old(_cost);
			apply(moves);

			out(OUT, ite, old);
		}
	} while (ite != maxIte && !stop);
}

void KMeansAlgo::apply(Moves const & moves) {
	if (!moves.empty()) {
		for (auto const & move : moves) {
			apply(move);
		}
		computeCenters(_centers);
		computeCost();
	}

}
void KMeansAlgo::apply(Move const & move) {
	size_t const i(move.first);
	size_t const from(_partition.label(i));
	size_t const to(move.second);
	// update centroids
	//	for (size_t d(0); d < _input.getM(); ++d) {
	//		_centers.plus(from, d, -_input.get(i, d));
	//		_centers.plus(to, d, _input.get(i, d));
	//	}
	//	_cost -= _d[i];
	//	_d[i] = move.second.second;
	//	_cost += _d[i];
	_partition.shift(i, to);
}

void KMeansAlgo::out(std::ostream & stream, size_t ite, Double old) const {
	stream << std::setw(6) << ite;
	stream << std::setw(6) << _partition.nbLabels();
	stream << std::setw(20) << std::setprecision(10) << _cost;
	if (old != Zero<Double>())
		stream << std::setw(20) << (old - _cost) / old * 100;
	stream << std::endl;
}
void KMeansAlgo::check(std::string const & text) const {
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
