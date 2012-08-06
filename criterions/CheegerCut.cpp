/*
 * CheegerCut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "CheegerCut.hpp"
#include "IGraph.hpp"

// calcul brut
double CheegerCut::eval(IExtendedPartition const & data) const {
	DoubleVector cut(data.nbNodes(), 0);
	getCut(data, cut);
	double value(0);
	double minSize(data.nbNodes());
	for (auto const & label : data.used()) {
		value += cut[label];
		minSize = (
				minSize > data.sizeOfLabel(label) ?
						data.sizeOfLabel(label) : minSize);
	}
	return value / minSize;
}
// calcul de la composante associé au label
double CheegerCut::eval(IExtendedPartition const & data,
		size_t const & l) const {
	double cut(getCut(data, l));
	double minSize(data.nbNodes());
	for (auto const & label : data.used())
		minSize = (
				minSize > data.sizeOfLabel(label) ?
						data.sizeOfLabel(label) : minSize);
	return cut / minSize;
}

CheegerCut::~CheegerCut() {
}
