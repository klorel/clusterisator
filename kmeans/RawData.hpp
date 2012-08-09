/*
 * RawData.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef RAWDATA_HPP_
#define RAWDATA_HPP_

#include <common.h>

class RawData {
public:
	RawData(size_t const & n = 0, size_t const & m = 0);
	virtual ~RawData();
public:
	void allocate(size_t const & n, size_t const & m);
	double get(size_t const & i, size_t const & j)const;
	double get(size_t const & i)const;
	size_t  getN()const;
	size_t  getM()const;
private:
	size_t _n;
	size_t _m;
	DoubleMatrix _observations;
};

#endif /* RAWDATA_HPP_ */
