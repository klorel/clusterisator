/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "RawData.hpp"

RawData::RawData(size_t const & m) {
	allocate(m);
}

RawData::~RawData() {

}
size_t RawData::getN() const {
	return _observations.size();
}
size_t RawData::getM() const {
	return _m;
}
void RawData::allocate(size_t const & m) {
	_m = m;
}
void RawData::allocate(size_t const & n, size_t const & m) {
	allocate(m);
	reserve(n);
}

void RawData::reserve(size_t const & n) {
	_observations.reserve(n);

}
Double RawData::get(size_t const & i, size_t const & j) const {
	Double dot_product(0);
	for (size_t k(0); k < getM(); ++k) {
		dot_product += _observations[i][k] * _observations[j][k];
	}
	return dot_product;
}
void RawData::add(std::vector<Double> const & v, Double  w) {
	_observations.push_back(std::vector<Double>(getM()));
	std::copy(v.begin(), v.end(), _observations.back().begin());
	_weights.push_back(w);
}

Double RawData::weight(size_t const & i) const {
	return _weights[i];
}

Double RawData::get(size_t const & i) const {
	return get(i, i);

}

void RawData::setWeights(DoubleVector const & weights) {
	assert(weights.size() == getN());
	_weights = weights;
}
