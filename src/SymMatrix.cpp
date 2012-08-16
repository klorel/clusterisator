/*
 * SymMatrix.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "SymMatrix.hpp"

SymMatrix::SymMatrix(size_t n) {
	allocate(n);
}

SymMatrix::~SymMatrix() {
}
void SymMatrix::allocate(size_t n) {
	_n = n;
	_matix.assign(n * (n - 1) / 2, Zero<Double>());
}

size_t SymMatrix::key(size_t i, size_t j) const {
	if (i < j)
		return key(j, i);
	else {
		return j * (j - 1) / 2 + i;
	}
}

Double SymMatrix::get(size_t i, size_t j) const {
	return _matix[key(i, j)];
}
void SymMatrix::set(size_t i, size_t j, Double v) {
	_matix[key(i, j)] = v;

}
