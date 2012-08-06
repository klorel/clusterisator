/*
 * Modularity.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "Modularity.hpp"
#include "IGraph.hpp"
#include "Data.hpp"

// calcul brut
double Modularity::eval(IExtendedPartition const & data) const {
	DoubleVector degree(data.nbNodes(), 0);
	DoubleVector intra(data.nbNodes(), 0);

	for (size_t n(0); n < data.nbNodes(); ++n) {
		size_t const & l(data.label(n));
		degree[l] += data.graph().degree(n);
		for (auto const & e : data.graph().adjacentList(n)) {
			if (data.label(e.first) == l)
				intra[l] += e.second;
		}
	}
	double value(0);
	double const & M(data.graph().degree());
	for (auto const & l : data.used())
		value += intra[l] / M - degree[l] / (M * M);
	return value;
}
// calcul de la composante associé au label
double Modularity::eval(IExtendedPartition const & data,
		size_t const & l) const {
	double degree(0);
	double intra(0);
	for (auto const & n : data.list(l)) {
		size_t const & l(data.label(n));
		degree += data.graph().degree(n);
		for (auto const & e : data.graph().adjacentList(n)) {
			if (data.label(e.first) == l)
				intra += e.second;
		}
	}
	double const & M(data.graph().degree());

	return intra / M - degree / (M * M);
}

Modularity::~Modularity() {
}
