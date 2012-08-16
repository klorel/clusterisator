/*
 * Acc.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Acc.hpp"
#include "../src/IPartition.hpp"

Acc::Acc() {
}

Acc::~Acc() {
}

Double Acc::compute(IPartition const & real, IPartition const & candidate) {
	// pas de traitement supplémentaire
	Double score(0);
	for (size_t i(0); i < real.nbObs(); ++i) {
		if (real.label(i) == candidate.label(i))
			++score;
	}
	return score / static_cast<Double>(real.nbObs());
}
