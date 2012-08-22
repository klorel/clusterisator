/*
 * RandIndex.hpp
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#ifndef RANDINDEX_HPP_
#define RANDINDEX_HPP_

#include "../src/IMesure.hpp"
#include "../src/common.h"
#include "../src/IPartition.hpp"

class RandIndex: public IMesure {
public:
	RandIndex();
	virtual ~RandIndex();
public:

	Double compute(IPartition const & real, IPartition const & candidate);
private:

};

inline RandIndex::RandIndex() {
}

inline RandIndex::~RandIndex() {
}

inline Double RandIndex::compute(IPartition const & real,
		IPartition const & candidate) {
	// true positive : same real - same proposed
	Double TP(Zero<Double>());
	// true negative : different real - different proposed
	Double TN(Zero<Double>());
	// false positive : same real - different proposed
	Double FP(Zero<Double>());
	// false negative : different real - same proposed
	Double FN(Zero<Double>());
	for (size_t i(0); i < real.nbObs(); ++i) {
		for (size_t j(i + 1); j < real.nbObs(); ++j) {
			if (real.label(i) == real.label(j)) {
				if (candidate.label(i) == candidate.label(j))
					++TP;
				else
					++FP;
			} else {
				if (candidate.label(i) != candidate.label(j))
					++TN;
				else
					++FN;
			}
		}
	}
	return (TP + TN) / (TP + TN + FP + FN);
}

#endif /* RANDINDEX_HPP_ */
