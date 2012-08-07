/*
 * Density.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "Density.hpp"
#include "Graph.hpp"
#include "Data.hpp"

double Density::eval(IExtendedPartition const & data) const {

	DoubleVector intra(data.nbNodes(), 0);
	DoubleVector cut(data.nbNodes(), 0);

	for (size_t n(0); n < data.nbNodes(); ++n) {
		size_t const & l(data.label(n));
		FOR_EACH_CONST(e , data.graph().adjacentList(n)) {
			if (data.label(e.first) == l)
				intra[l] += e.second;
			else
				cut[l] += e.second;
		}
	}
	double value(0);
	FOR_EACH_CONST(l , data.used()) {
		value += (intra[l] - cut[l] / 2.0)
				/ static_cast<double>(data.sizeOfLabel(l));
	}
	return value;
}

double Density::eval(IExtendedPartition const & data, size_t const & l) const {
	double cut(0);
	double intra(0);
	FOR_EACH_CONST (n , data.list(l)) {
		size_t const & l(data.label(n));
		FOR_EACH_CONST(e , data.graph().adjacentList(n)) {
			if (data.label(e.first) == l)
				intra += e.second;
			else
				cut += e.second;

		}
	}

	return (intra - cut) / static_cast<double>(data.sizeOfLabel(l));
}
Density::~Density() {

}
