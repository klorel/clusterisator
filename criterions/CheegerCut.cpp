/*
 * CheegerCut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "CheegerCut.hpp"
#include "IGraphPartition.hpp"
#include "ILinks.hpp"



double CheegerCut::eval(IGraphPartition const & data) const {
	DoubleVector cut(data.nbObs(), 0);
	getCut(data, cut);
	double value(0);
	double minSize(data.nbObs());
	FOR_EACH_CONST(label ,data.used()) {
		value += cut[label];
		minSize = (
				minSize > data.sizeOfLabel(label) ?
						data.sizeOfLabel(label) : minSize);
	}
	return value / minSize;
}

double CheegerCut::eval(IGraphPartition const & data,
		size_t const & l) const {
	double cut(getCut(data, l));
	double minSize(data.nbObs());
	FOR_EACH_CONST(label ,data.used()) {
		minSize = (
				minSize > data.sizeOfLabel(label) ?
						data.sizeOfLabel(label) : minSize);
	}
	return cut / minSize;
}

CheegerCut::~CheegerCut() {
}
