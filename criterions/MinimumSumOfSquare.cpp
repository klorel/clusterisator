/*
 * Mssc.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "MinimumSumOfSquare.hpp"
#include "IGraphPartition.hpp"
#include "ILinks.hpp"

double MinimumSumOfSquare::eval(IGraphPartition const & data) const {
	DoubleVector cut(data.nbObs(), 0);
	getCut(data, cut);
	double value(0);
	FOR_EACH_CONST(label , data.used()) {
		value += cut[label] / static_cast<double>(data.sizeOfLabel(label));
	}
	return value;
}

double MinimumSumOfSquare::eval(IGraphPartition const & data,
		size_t const & label) const {
	double cut(getCut(data, label));
	return cut / static_cast<double>(data.sizeOfLabel(label));
}

MinimumSumOfSquare::~MinimumSumOfSquare() {
}
