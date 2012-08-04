/*
 * Density.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "Density.hpp"
#include "Graph.hpp"
#include "Data.hpp"

// calcul brut
double Density::eval(IExtendedPartition const & data) const {

	DoubleVector intra(data.nbNodes(), 0);
	DoubleVector cut(data.nbNodes(), 0);

	for (size_t n(0); n < data.nbNodes(); ++n) {
		size_t const & l(data.label(n));
		for (auto const & e : data.graph().adjacentList(n)) {
			if (data.label(e.first) == l)
				intra[l] += e.second;
			else
				cut[l] += e.second;
		}
	}
	double value(0);
	for (auto const & l : data.used()) {

		value += (intra[l] - cut[l] / 2.0)
				/ static_cast<double>(data.sizeOfLabel(l));
	}
	return value;
}
// calcul de la composante associé au label
double Density::eval(IExtendedPartition const & data, size_t const & l) const {
	double cut(0);
	double intra(0);
	for (auto const & n : data.list(l)) {
		size_t const & l(data.label(n));
		for (auto const & e : data.graph().adjacentList(n)) {
			if (data.label(e.first) == l)
				intra += e.second;
			else
				cut += e.second;

		}
	}

	return (intra - cut) / static_cast<double>(data.sizeOfLabel(l));
}
bool Density::isBetter(double const & candidate, double const & ref) const {
	return candidate > 1e-10 + ref;

}

bool Density::isPartitioning() const {
	return false;
}
Density::~Density() {

}
