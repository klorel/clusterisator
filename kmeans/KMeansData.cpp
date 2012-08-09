/*
 * KMeansData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "KMeansData.hpp"

KMeansData::KMeansData(size_t const & n, size_t const & m, size_t const & k) :
		RawData(n, m), _k(k) {
	buildDiag();
}

KMeansData::~KMeansData() {
}

void KMeansData::buildDiag() {
	_kii.assign(getN(), 0);
	for (size_t i(0); i < getN(); ++i)
		_kii[i] += get(i);
}
void KMeansData::init(IntVector const & x) {
	_x = x;
	_sumK.assign(_k, 0);
	_sumW.assign(_k, 0);
	_list.assign(_k, IntList());
	for (size_t i(0); i < getN(); ++i) {
		size_t const c(_x[i]);
		_sumK[c] += _kii[i] * _weights[i];
		_sumW[c] += _weights[i];
		_list[c].push_front(i);
		_position[i] = _list.begin();
	}
}
void KMeansData::setWeights(DoubleVector const & weights) {
	_weights = weights;
}
bool KMeansData::loop() {
	return true;
}
size_t KMeansData::getK() const {
	return _k;
}
size_t KMeansData::loop(size_t const & i) {
	_buffer.resize(_k);
	return 0;
}

double KMeansData::d(size_t const & i, size_t const & c) const {
	double result(0);
	result += _kii[i];
	result -= (2 * _sumK[c] / _sumW[c]);

	return result;
}
void KMeansData::initLoop() {
	_buffer.assign(getK(), 0);
	for (size_t k(0); k < getK(); ++k) {

	}
}
void KMeansData::move(size_t const & i, size_t const & c) {
	if (_x[i] != c) {
		_sumW[_x[i]] -= _weights[i];
		_list[_x[i]].erase(_position[i]);

		_sumW[c] += _weights[i];
		_list[c].push_back(i);

		_position[i] = _list[c].begin();
		_x[i] = c;
	}
}
