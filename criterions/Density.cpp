/*
 * Density.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "Density.hpp"
#include "../interfaces/IGraphPartition.hpp"
#include "../interfaces/ILinks.hpp"

Double Density::eval(IGraphPartition const & data) const {

	DoubleVector intra(data.nbObs(), 0);
	DoubleVector cut(data.nbObs(), 0);

	//	for (size_t n(0); n < data.nbObs(); ++n) {
	//		size_t const & l(data.label(n));
	//		FOR_EACH_CONST(e , data.graph().links(n)) {
	//			if (data.label(e.first) == l)
	//				intra[l] += e.second;
	//			else
	//				cut[l] += e.second;
	//		}
	//	}
	Double value(0);
	//	FOR_EACH_CONST(l , data.used()) {
	//		value += (intra[l] - cut[l] / 2.0)
	//				/ static_cast<Double>(data.sizeOfLabel(l));
	//	}
	return value;
}

Double Density::eval(IGraphPartition const & data, size_t const & l) const {
	Double cut(0);
	Double intra(0);
	//	FOR_EACH_CONST (n , data.list(l)) {
	//		size_t const & l(data.label(n));
	//		FOR_EACH_CONST(e , data.graph().links(n)) {
	//			if (data.label(e.first) == l)
	//				intra += e.second;
	//			else
	//				cut += e.second;
	//
	//		}
	//	}

	return (intra - cut) / static_cast<Double>(data.sizeOfLabel(l));
}
Density::~Density() {

}
