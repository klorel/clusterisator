/*
 * CheegerCut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "CheegerCut.hpp"
#include "IGraphPartition.hpp"
#include "ILinks.hpp"

Double CheegerCut::eval(IGraphPartition const & data) const {
	DoubleVector cut(data.nbObs(), 0);
	getCut(data, cut);
	Double value(0);
	Double minSize(data.nbObs());
	for (auto const & label : data.used()) {
		value += cut[label];
		minSize = (
				minSize > data.sizeOfLabel(label) ?
						data.sizeOfLabel(label) : minSize);
	}
	return value / minSize;
}

Double CheegerCut::eval(IGraphPartition const & data, size_t const & l) const {
	Double cut(getCut(data, l));
	Double minSize(data.nbObs());
	for (auto const & label : data.used()) {
		minSize = (
				minSize > data.sizeOfLabel(label) ?
						data.sizeOfLabel(label) : minSize);
	}
	return cut / minSize;
}

CheegerCut::~CheegerCut() {
}
