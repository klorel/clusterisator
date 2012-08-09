/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "RawData.hpp"

RawData::RawData(size_t const & n, size_t const & m) {
	allocate(n, m);

}

RawData::~RawData() {

}
size_t RawData::getN() const {
	return _n;
}
size_t RawData::getM() const {
	return _m;
}
void RawData::allocate(size_t const & n, size_t const & m) {
	_n = n;
	_m = m;
	_observations.assign(n, DoubleVector(m, 0));
}

double RawData::get(size_t const & i, size_t const & j) const {
	double dot_product(0);
	for (size_t k(0); k < getM(); ++k) {
		dot_product += _observations[i][k] * _observations[j][k];
	}
	return dot_product;
}
double RawData::get(size_t const & i) const {
	return get(i, i);
}
