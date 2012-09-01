/*
 * Acc.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef ACC_HPP_
#define ACC_HPP_

#include "src/common.h"
#include "src/IMeasure.hpp"
#include "src/IPartition.hpp"

class Acc: public IMeasure {
  public:
    virtual ~Acc();

    Double compute(IPartition const & actual, IPartition const & candidate);
};

Acc::~Acc() {}

Double Acc::compute(IPartition const & actual, IPartition const & candidate) {
	Double score(0);
	for (size_t i(0); i < actual.nbObs(); ++i) {
		if (actual.label(i) == candidate.label(i))
			++score;
	}
	return score / static_cast<Double>(actual.nbObs());
}

#endif /* ACC_HPP_ */
