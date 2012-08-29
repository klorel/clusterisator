/*
 * RectMatrix.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef RECTMATRIX_HPP_
#define RECTMATRIX_HPP_

#include "src/common.h"

/**
 * Represent a rectangular matrix of Double
 */
class RectMatrix {
public:
	RectMatrix(size_t n = 0, size_t m = 0, Double v = Zero<Double>());
	RectMatrix(size_t n, size_t, DoubleVector const&);
	virtual ~RectMatrix();

  /**
	 * Reset the dimension and the content of the matrix. Set every coefficient to the same value
	 */
	void allocate(size_t n, size_t m, Double v = Zero<Double>());

	Double get(size_t i, size_t j) const;
	Double & get(size_t i, size_t j);
	void set(size_t i, size_t j, Double v);

	/**
	 * Add the given value to the given element
	 */
	void plus(size_t i, size_t j, Double v);

	size_t getN() const;
	size_t getM() const;

	DoubleVector const & matrix() const;

	/**
	 * Set every coefficient to the given value
	 */
	void assign(Double v);

	bool operator!=(RectMatrix const & rhs) const;
	bool operator==(RectMatrix const & rhs) const;

	void read(std::string const &);
	void read(size_t i, std::string const &);
private:
	size_t key(size_t i, size_t j) const;
	size_t _n;
	size_t _m;
	DoubleVector _matrix;
};

std::ostream & operator<<(std::ostream &, RectMatrix const &);

inline RectMatrix::RectMatrix(size_t n, size_t m, Double v) {
	allocate(n, m, v);

}
inline RectMatrix::RectMatrix(size_t n, size_t m, DoubleVector const &matrix) :
		_n(n), _m(m), _matrix(matrix) {
  assert(n*m == matrix.size());
}
inline RectMatrix::~RectMatrix() {

}

inline void RectMatrix::allocate(size_t n, size_t m, Double v) {
	_n = n;
	_m = m;
	_matrix.assign(n * m, v);
}

inline size_t RectMatrix::key(size_t i, size_t j) const {
	return i * _m + j;
}

inline Double RectMatrix::get(size_t i, size_t j) const {
	return _matrix[key(i, j)];
}
inline Double & RectMatrix::get(size_t i, size_t j) {
	return _matrix[key(i, j)];
}
inline void RectMatrix::set(size_t i, size_t j, Double v) {
	_matrix[key(i, j)] = v;
}

inline void RectMatrix::plus(size_t i, size_t j, Double v) {
	_matrix[key(i, j)] += v;
}
inline size_t RectMatrix::getN() const {
	return _n;
}
inline size_t RectMatrix::getM() const {
	return _m;
}

inline void RectMatrix::assign(Double v) {
	std::fill_n(_matrix.begin(), _matrix.size(), v);
}

inline std::ostream & operator<<(std::ostream & stream,
		RectMatrix const & rhs) {
	stream << rhs.getN() << " " << rhs.getM() << std::endl;
	for (size_t i(0); i < rhs.getN(); ++i) {
		for (size_t j(0); j < rhs.getM(); ++j) {
			stream << rhs.get(i, j) << " ";
		}
		stream << std::endl;
	}
	return stream;
}
inline bool RectMatrix::operator!=(RectMatrix const & rhs) const {
	return !((*this) == rhs);
}
inline bool RectMatrix::operator==(RectMatrix const & rhs) const {
	for (size_t i(0); i < rhs._matrix.size(); ++i)
		if (!IsEqual(_matrix[i], rhs._matrix[i]))
			return false;
	return true;
}
inline void RectMatrix::read(size_t i, std::string const & values) {
	std::stringstream stream(values);
	for (size_t d(0); d < _m; ++d)
		stream >> get(i, d);
}

inline void RectMatrix::read(std::string const & values) {
	std::stringstream stream(values);
	for (size_t i(0); i < _n; ++i)
		for (size_t d(0); d < _m; ++d)
			stream >> get(i, d);

}

inline DoubleVector const &RectMatrix::matrix() const {
	return _matrix;
}
#endif /* RECTMATRIX_HPP_ */
