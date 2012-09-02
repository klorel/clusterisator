/*
 * SymMatrix.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef SYMMATRIX_HPP_
#define SYMMATRIX_HPP_

#include "src/common.h"
#include <algorithm>
#include <vector>

/**
 * Represent a symetric, real-valued, matrix
 */
class SymMatrix {
  public:
    /**
     * @param n Size of the matrix
     */
    SymMatrix(size_t n);
    virtual ~SymMatrix();

    Double get(size_t i, size_t j) const;
    void set(size_t i, size_t j, Double v);

  private:
    /**
     * (Re)initialize the matrix: its size is set equal to n, and is coefficients are set to 0
     */
    void allocate(size_t n);

    /**
     * The size of the matrix
     */
    size_t _n;

    /**
     * The actual, needed, coefficients
     */
    std::vector<DoubleVector> _matrix;
};

SymMatrix::SymMatrix(size_t n) {
  allocate(n);
}

SymMatrix::~SymMatrix() {
}
inline void SymMatrix::allocate(size_t n) {
  _n = n;
  _matrix = std::vector<DoubleVector>(n);
  for ( size_t idx=0 ; idx < n ; idx++ ){
    _matrix[idx] = DoubleVector(idx+1, Zero<Double>());
  }
}

inline Double SymMatrix::get(size_t i, size_t j) const {
  assert ( i < _n && j < _n);
  return _matrix[std::max(i, j)][std::min(i, j)];
}
inline void SymMatrix::set(size_t i, size_t j, Double v) {
  assert ( i < _n && j < _n);
  _matrix[std::max(i, j)][std::min(i, j)] = v;
}

#endif /* SYMMATRIX_HPP_ */
