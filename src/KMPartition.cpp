/*
 * KMPartition.cpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#include "KMPartition.hpp"

KMPartition::KMPartition(KMInstance const & input, size_t k) :
		Partition(input.nbObs(), k), _input(input), _centers(k, input.nbAtt()) {
	setWeights(_input.weights());
	computeCenters();
}
KMPartition::KMPartition(KMInstance const &input, Partition const &rhs) :
		Partition(rhs), _input(input), _centers(rhs.maxNbLabels(),
				input.nbAtt()) {
	setWeights(_input.weights());
	computeCenters();
}
KMPartition::~KMPartition() {

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
void KMPartition::shift(size_t node, size_t to) {
	size_t const from(label(node));
	// recomputing centers is needed
	if (_centers.getN() != maxNbLabels()) {
		computeCenters();
	} else {
		for (size_t d(0); d < _input.nbAtt(); ++d) {
			_centers.plus(from, d, -_input.get(node, d) * obsWeight(node));
			_centers.plus(to, d, _input.get(node, d) * obsWeight(node));
		}
	}
	Partition::shift(node, to);
}
