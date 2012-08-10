/*
 * IRawData.hpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#ifndef IRAWDATA_HPP_
#define IRAWDATA_HPP_

#include <iostream>
#include <iostream>

class IRawData {
public:
	virtual size_t getN() const = 0;
	virtual size_t getM() const = 0;

	virtual double get(size_t const & i, size_t const & j) const = 0;
	virtual double get(size_t const & i) const = 0;

	virtual double weight(size_t const & i) const = 0;
	virtual void setWeights(DoubleVector const &) = 0;

	virtual void allocate(size_t const & m) = 0;
	virtual void allocate(size_t const & n, size_t const & m) = 0;
	virtual void reserve(size_t const & n) = 0;

	virtual void add(std::vector<double> const &, double const & w = 1.0) = 0;
	void add(std::istream &);

	virtual ~IRawData();
};

inline IRawData::~IRawData() {
}
inline void IRawData::add(std::istream & stream) {
	std::vector<double> v(getM());
	for (size_t i(0); i < getM(); ++i)
		stream >> v[i];
	add(v);
}
#endif /* IRAWDATA_HPP_ */
