/*
 * Mssc.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "MinimumSumOfSquare.hpp"

// calcul brut
double MinimumSumOfSquare::eval(IExtendedPartition const & data) const {
	DoubleVector cut(data.nbNodes(), 0);
	getCut(data, cut);
	double value(0);
	for (auto const & label : data.used()) {
		value += cut[label] / static_cast<double>(data.sizeOfLabel(label));
	}
	return value;
}
// calcul de la composante associé au label
double MinimumSumOfSquare::eval(IExtendedPartition const & data,
		size_t const & label) const {
	double cut(getCut(data, label));
	return cut / static_cast<double>(data.sizeOfLabel(label));
}

MinimumSumOfSquare::~MinimumSumOfSquare() {
}
