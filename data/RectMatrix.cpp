/*
 * RectMatrix.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "RectMatrix.hpp"

RectMatrix::RectMatrix(size_t n, size_t m) {
	allocate(n, m);

}

RectMatrix::~RectMatrix() {

}

void RectMatrix::allocate(size_t n, size_t m) {
	_n = n;
	_m = m;
	_matrix.assign(n * m, Zero<Double>());
}

size_t RectMatrix::key(size_t i, size_t j) const {
	return i * _m + j;
}

Double RectMatrix::get(size_t i, size_t j) const {
	return _matrix[key(i, j)];
}
Double & RectMatrix::get(size_t i, size_t j) {
	return _matrix[key(i, j)];
}
void RectMatrix::set(size_t i, size_t j, Double v) {
	_matrix[key(i, j)] = v;
}

void RectMatrix::plus(size_t i, size_t j, Double v) {
	_matrix[key(i, j)] += v;
}
size_t RectMatrix::getN() const {
	return _n;
}
size_t RectMatrix::getM() const {
	return _m;
}

void RectMatrix::assign(Double v) {
	std::fill_n(_matrix.begin(), _matrix.size(), v);
//	_matrix.assign(_matrix.size(), v);
}
Double RectMatrix::get(size_t i, DoubleVector const & point) const {
	Double square_distance(0);
	for (size_t d(0); d < getM(); ++d) {
		square_distance += std::pow(get(i, d) - point[d], 2);
	}
	return square_distance;
}

std::ostream & operator<<(std::ostream & stream, RectMatrix const & rhs) {
	stream << rhs.getN() << " " << rhs.getM() << "\n";
	for (size_t i(0); i < rhs.getN(); ++i) {
		for (size_t j(0); j < rhs.getM(); ++j) {
			stream << rhs.get(i, j) << " ";
		}
		stream << "\n";
	}
	return stream;
}
bool RectMatrix::operator!=(RectMatrix const & rhs) const {
	return !((*this) == rhs);
}
bool RectMatrix::operator==(RectMatrix const & rhs) const {
	return (_matrix == rhs._matrix);
}
