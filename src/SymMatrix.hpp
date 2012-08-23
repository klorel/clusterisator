/*
 * SymMatrix.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef SYMMATRIX_HPP_
#define SYMMATRIX_HPP_

#include "src/common.h"

class SymMatrix {
public:
	SymMatrix(size_t n);
	virtual ~SymMatrix();

	void allocate(size_t n);

	Double get(size_t i, size_t j) const;
	void set(size_t i, size_t j, Double v);
private:
	size_t key(size_t i, size_t j) const;
	size_t _n;
	DoubleVector _matix;
};

SymMatrix::SymMatrix(size_t n) {
	allocate(n);
}

SymMatrix::~SymMatrix() {
}
inline void SymMatrix::allocate(size_t n) {
	_n = n;
	_matix.assign(n * (n - 1) / 2, Zero<Double>());
}

inline size_t SymMatrix::key(size_t i, size_t j) const {
	if (i < j)
		return key(j, i);
	else {
		return j * (j - 1) / 2 + i;
	}
}

inline Double SymMatrix::get(size_t i, size_t j) const {
	return _matix[key(i, j)];
}
inline void SymMatrix::set(size_t i, size_t j, Double v) {
	_matix[key(i, j)] = v;

}

#endif /* SYMMATRIX_HPP_ */
