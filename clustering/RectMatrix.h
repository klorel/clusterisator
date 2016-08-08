/*
 * RectMatrix.h
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef RECTMATRIX_HPP_
#define RECTMATRIX_HPP_

#include "common.h"

/**
 * Represent a rectangular matrix of Double
 */
class RectMatrix {
public:
	RectMatrix(int n = 0, int m = 0, Double v = 0);
	RectMatrix(int n, int, DoubleVector const&);
	virtual ~RectMatrix();

	/**
	 * Reset the dimension and the content of the matrix. Set every coefficient to the same value
	 */
	void allocate(int n, int m, Double v = 0);

	Double get(int i, int j) const;
	Double & get(int i, int j);
	void set(int i, int j, Double v);

	/**
	 * Add the given value to the given element
	 */
	void plus(int i, int j, Double v);

	int getN() const;
	int getM() const;

	DoubleVector const & matrix() const;

	/**
	 * Set every coefficient to the given value
	 */
	void assign(Double v);

	bool operator!=(RectMatrix const & rhs) const;
	bool operator==(RectMatrix const & rhs) const;

	void read(std::string const &);
	void read(int i, std::string const &);

	DoubleVector::const_iterator begin() const;
	DoubleVector::const_iterator end() const;
private:
	int key(int i, int j) const;
	int _n;
	int _m;
	DoubleVector _matrix;
};

std::ostream & operator<<(std::ostream &, RectMatrix const &);
void operator<<(RectMatrix &matrix, std::istream & stream);

inline RectMatrix::RectMatrix(int n, int m, Double v) {
	allocate(n, m, v);

}
inline RectMatrix::RectMatrix(int n, int m, DoubleVector const &matrix) :
		_n(n), _m(m), _matrix(matrix) {
	assert(n * m == matrix.size());
}
inline RectMatrix::~RectMatrix() {

}

inline void RectMatrix::allocate(int n, int m, Double v) {
	_n = n;
	_m = m;
	_matrix.assign(n * m, v);
}

inline int RectMatrix::key(int i, int j) const {
	return i * _m + j;
}

inline Double RectMatrix::get(int i, int j) const {
	return _matrix[key(i, j)];
}
inline Double & RectMatrix::get(int i, int j) {
	return _matrix[key(i, j)];
}
inline void RectMatrix::set(int i, int j, Double v) {
	_matrix[key(i, j)] = v;
}

inline void RectMatrix::plus(int i, int j, Double v) {
	_matrix[key(i, j)] += v;
}
inline int RectMatrix::getN() const {
	return _n;
}
inline int RectMatrix::getM() const {
	return _m;
}

inline void RectMatrix::assign(Double v) {
	std::fill_n(_matrix.begin(), _matrix.size(), v);
}

inline std::ostream & operator<<(std::ostream & stream,
		RectMatrix const & rhs) {
	stream << rhs.getN() << " " << rhs.getM() << std::endl;
	for (int i(0); i < rhs.getN(); ++i) {
		for (int j(0); j < rhs.getM(); ++j) {
			stream << rhs.get(i, j) << " ";
		}
		stream << std::endl;
	}
	return stream;
}
inline void operator<<(RectMatrix &matrix, std::istream & stream) {
	std::string line;
	std::getline(stream, line);
	int n;
	int m;
	{
		std::stringstream buffer(line);
		buffer >> n;
		buffer >> m;
	}
	matrix.allocate(n, m);
	for (int i(0); i < n; ++i) {
		std::getline(stream, line);
		std::stringstream buffer(line);
		for (int j(0); j < m; ++j) {
			buffer >> matrix.get(i, j);
		}
	}
	//std::cout << matrix<<std::endl;
}
inline bool RectMatrix::operator!=(RectMatrix const & rhs) const {
	return !((*this) == rhs);
}
inline bool RectMatrix::operator==(RectMatrix const & rhs) const {
	for (int i(0); i < rhs._matrix.size(); ++i)
		if (!IsEqual(_matrix[i], rhs._matrix[i]))
			return false;
	return true;
}
inline void RectMatrix::read(int i, std::string const & values) {
	std::stringstream stream(values);
	for (int d(0); d < _m; ++d)
		stream >> get(i, d);
}

inline void RectMatrix::read(std::string const & values) {
	std::stringstream stream(values);
	for (int i(0); i < _n; ++i)
		for (int d(0); d < _m; ++d)
			stream >> get(i, d);

}

inline DoubleVector const &RectMatrix::matrix() const {
	return _matrix;
}

inline DoubleVector::const_iterator RectMatrix::begin() const {
	return _matrix.begin();
}
inline DoubleVector::const_iterator RectMatrix::end() const {
	return _matrix.end();
}

#endif /* RECTMATRIX_HPP_ */
