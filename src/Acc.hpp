/*
 * Acc.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef ACC_HPP_
#define ACC_HPP_

#include "../src/common.h"
#include "../src/IMesure.hpp"
#include "../src/IPartition.hpp"

class Acc: public IMesure {
public:
	Acc();
	virtual ~Acc();
public:

	Double compute(IPartition const & real, IPartition const & candidate);
};

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

#endif /* ACC_HPP_ */
