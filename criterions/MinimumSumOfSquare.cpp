/*
 * Mssc.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "MinimumSumOfSquare.hpp"
#include "../interfaces/ISolution.hpp"
#include "../interfaces/ILinks.hpp"

Double MinimumSumOfSquare::eval(ISolution const & solution) const {
//	DoubleVector cut(solution.partition().nbObs(), 0);
//	getCut(solution, cut);
	Double value(0);
//	for (auto const & label : solution.partition().used()) {
//		value += cut[label]
//				/ static_cast<Double>(solution.partition().sizeOfLabel(label));
//	}
	return value;
}

Double MinimumSumOfSquare::eval(ISolution const & solution,
		size_t const & label) const {
//	Double cut(getCut(solution, label));
//	return cut / static_cast<Double>(solution.partition().sizeOfLabel(label));
	return 0;
}

MinimumSumOfSquare::~MinimumSumOfSquare() {
}
