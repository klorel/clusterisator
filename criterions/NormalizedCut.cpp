/*
 * NormalizedCut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "NormalizedCut.hpp"


double NormalizedCut::eval(IExtendedPartition const & data) const {
	DoubleVector cut(data.nbNodes(), 0);
	getCut(data, cut);
	double value(0);
	FOR_EACH_CONST( label ,data.used()) {
		value += cut[label] / data.degreeOfLabel(label);
	}
	return value;
}


double NormalizedCut::eval(IExtendedPartition const & data,
		size_t const & label) const {
	double cut(getCut(data, label));
	return cut / data.degreeOfLabel(label);
}

NormalizedCut::~NormalizedCut() {
}
