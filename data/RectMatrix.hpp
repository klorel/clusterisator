/*
 * RectMatrix.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef RECTMATRIX_HPP_
#define RECTMATRIX_HPP_

#include "../utils/common.h"

class RectMatrix {
public:
	RectMatrix(size_t n = 0, size_t m = 0);
	virtual ~RectMatrix();

	void allocate(size_t n, size_t m);

	Double get(size_t i, size_t j) const;
	void set(size_t i, size_t j, Double v);
	void plus(size_t i, size_t j, Double v);
	Double get(size_t i, DoubleVector const & point) const;

	size_t getN() const;
	size_t getM() const;

DoubleVector const& getRow(size_t i)const;
	void assign(Double v);
private:
	size_t key(size_t i, size_t j) const;
	size_t _n;
	size_t _m;
	DoubleVector _matix;
};

std::ostream & operator<<(std::ostream &, RectMatrix const &);
#endif /* RECTMATRIX_HPP_ */
