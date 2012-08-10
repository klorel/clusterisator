/*
 * Acc.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Acc.hpp"
#include "IPartition.hpp"

Acc::Acc() {
}

Acc::~Acc() {
}

double Acc::compute(IPartition const & real, IPartition const & candidate) {
	// pas de traitement supplémentaire
	double score(0);
	for (size_t i(0); i < real.nbObs(); ++i) {
		if (real.label(i) == candidate.label(i))
			++score;
	}
	return score / static_cast<double>(real.nbObs());
}
