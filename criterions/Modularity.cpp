/*
 * Modularity.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "Modularity.hpp"
#include "IGraphPartition.hpp"
#include "ILinks.hpp"

Double Modularity::eval(IGraphPartition const & data) const {
	//	DoubleVector degree(data.nbNodes(), 0);
	//	DoubleVector intra(data.nbNodes(), 0);
	//
	//	for (size_t n(0); n < data.nbNodes(); ++n) {
	//		size_t const & l(data.label(n));
	//		degree[l] += data.graph().weight(n);
	//		//		FOR_EACH_CONST(e , data.graph().links(n)) {
	//		//			if (data.label(e.first) == l)
	//		//				intra[l] += e.second;
	//		//		}
	//	}
	//	Double value(0);
	//	//	Double  M(data.graph().weight());
	//	//	FOR_EACH_CONST( l , data.used()) value += intra[l] / M - degree[l] / (M * M);
	//	return value;
	return Zero<Double>();
}

Double Modularity::eval(IGraphPartition const & data, size_t const & l) const {
	//	Double degree(0);
	//	Double intra(0);
	//	FOR_EACH_CONST(n, data.list(l)) {
	//		size_t const & l(data.label(n));
	//		degree += data.graph().weight(n);
	//		FOR_EACH_CONST(e , data.graph().links(n)) {
	//			if (data.label(e.first) == l)
	//				intra += e.second;
	//		}
	//	}
	//	Double  M(data.graph().weight());

	//	return intra / M - degree / (M * M);
	return Zero<Double>();
}

Modularity::~Modularity() {
}
