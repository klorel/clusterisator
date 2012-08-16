/*
 * RectMatrix.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef RECTMATRIX_HPP_
#define RECTMATRIX_HPP_

#include "../src/common.h"

class RectMatrix {
public:
	RectMatrix(size_t n = 0, size_t m = 0);
	virtual ~RectMatrix();

	void allocate(size_t n, size_t m);

	Double get(size_t i, size_t j) const;
	Double & get(size_t i, size_t j);
	void set(size_t i, size_t j, Double v);
	void plus(size_t i, size_t j, Double v);
	Double get(size_t i, DoubleVector const & point) const;

	size_t getN() const;
	size_t getM() const;

	DoubleVector const& getRow(size_t i) const;
	void assign(Double v);

	bool operator!=(RectMatrix const & rhs) const;
	bool operator==(RectMatrix const & rhs) const;

private:
	size_t key(size_t i, size_t j) const;
	size_t _n;
	size_t _m;
	DoubleVector _matrix;
};

std::ostream & operator<<(std::ostream &, RectMatrix const &);

inline RectMatrix::RectMatrix(size_t n, size_t m) {
	allocate(n, m);

}

inline RectMatrix::~RectMatrix() {

}

inline void RectMatrix::allocate(size_t n, size_t m) {
	_n = n;
	_m = m;
	_matrix.assign(n * m, Zero<Double>());
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
//	_matrix.assign(_matrix.size(), v);
}
inline Double RectMatrix::get(size_t i, DoubleVector const & point) const {
	Double square_distance(0);
	for (size_t d(0); d < getM(); ++d) {
		square_distance += std::pow(get(i, d) - point[d], 2);
	}
	return square_distance;
}

inline std::ostream & operator<<(std::ostream & stream,
		RectMatrix const & rhs) {
	stream << rhs.getN() << " " << rhs.getM() << "\n";
	for (size_t i(0); i < rhs.getN(); ++i) {
		for (size_t j(0); j < rhs.getM(); ++j) {
			stream << rhs.get(i, j) << " ";
		}
		stream << "\n";
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

#endif /* RECTMATRIX_HPP_ */
