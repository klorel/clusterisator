/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

Double KMAlgo::ComputeMssc(IPartition const & x, KMInstance const & instance) {
	RectMatrix centers(x.maxNbLabels(), instance.nbAtt());
	centers.assign(0);
	DoubleVector weights(x.maxNbLabels(), 0);
	for (auto const & l : x.usedLabels()) {
		for (auto const & i : x.observations(l)) {
			weights[l] += instance.weight(i);
			for (size_t d(0); d < instance.nbAtt(); ++d)
				centers.plus(l, d, instance.get(i, d) * instance.weight(i));
		}
	}
	Double result(0);
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
