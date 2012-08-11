/*
 * RawData.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef RAWDATA_HPP_
#define RAWDATA_HPP_

#include <common.h>

#include "IRawData.hpp"

class RawData: public IRawData {
public:
	RawData(size_t const & m = 0);
	virtual ~RawData();
public:
	void allocate(size_t const & m);
	void allocate(size_t const & n, size_t const & m);
	void reserve(size_t const & n);
	Double get(size_t const & i, size_t const & j) const;
	Double get(size_t const & i) const;
	size_t getN() const;
	size_t getM() const;

	void add(std::vector<Double> const &, Double  w = 1.0);
	Double weight(size_t const & i) const;
	void setWeights(DoubleVector const & weights);
private:
	size_t _m;
	DoubleMatrix _observations;
	DoubleVector _weights;
};

#endif /* RAWDATA_HPP_ */
