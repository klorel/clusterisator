/*
 * Mssc.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "MinimumSumOfSquare.hpp"
#include "../interfaces/IGraphPartition.hpp"
#include "../interfaces/ILinks.hpp"

Double MinimumSumOfSquare::eval(IGraphPartition const & data) const {
	DoubleVector cut(data.nbObs(), 0);
	getCut(data, cut);
	Double value(0);
	for (auto const & label : data.used()) {
		value += cut[label] / static_cast<Double>(data.sizeOfLabel(label));
	}
	return value;
}

Double MinimumSumOfSquare::eval(IGraphPartition const & data,
		size_t const & label) const {
	Double cut(getCut(data, label));
	return cut / static_cast<Double>(data.sizeOfLabel(label));
}

MinimumSumOfSquare::~MinimumSumOfSquare() {
}
