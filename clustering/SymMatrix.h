/*
 * SymMatrix.h
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef SYMMATRIX_HPP_
#define SYMMATRIX_HPP_

#include "common.h"
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
  SymMatrix(int n);
  virtual ~SymMatrix();

  Double get(int i, int j) const;
  void set(int i, int j, Double v);

 private:
  /**
   * (Re)initialize the matrix: its size is set equal to n, and is coefficients are set to 0
   */
  void allocate(int n);

  /**
   * The size of the matrix
   */
  int _n;

  /**
   * The actual, needed, coefficients
   */
  std::vector<DoubleVector> _matrix;
};

SymMatrix::SymMatrix(int n) {
  allocate(n);
}

SymMatrix::~SymMatrix() {
}
inline void SymMatrix::allocate(int n) {
  _n = n;
  _matrix = std::vector<DoubleVector>(n);
  for (int idx = 0; idx < n; idx++) {
    _matrix[idx] = DoubleVector(idx + 1, Zero<Double>());
  }
}

inline Double SymMatrix::get(int i, int j) const {
  assert(i < _n && j < _n);
  return _matrix[std::max(i, j)][std::min(i, j)];
}
inline void SymMatrix::set(int i, int j, Double v) {
  assert(i < _n && j < _n);
  _matrix[std::max(i, j)][std::min(i, j)] = v;
}

#endif /* SYMMATRIX_HPP_ */
