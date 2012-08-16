/*
 * SymMatrix.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef SYMMATRIX_HPP_
#define SYMMATRIX_HPP_

#include "../src/common.h"

class SymMatrix {
public:
	SymMatrix(size_t n);
	virtual ~SymMatrix();

	void allocate(size_t n);

	Double get(size_t i, size_t j) const;
	void set(size_t i, size_t j, Double v);
private:
	size_t key(size_t i, size_t j )const;
	size_t _n;
	DoubleVector _matix;
};

#endif /* SYMMATRIX_HPP_ */
