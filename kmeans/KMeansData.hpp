/*
 * KMeansData.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef KMEANSDATA_HPP_
#define KMEANSDATA_HPP_

#include "RawData.hpp"

class KMeansData: public RawData {
public:
	KMeansData(size_t const & n, size_t const & m, size_t const & k);
	virtual ~KMeansData();
public:
	bool loop();
	size_t loop(size_t const & i);
	size_t getK() const;
	void setWeights(DoubleVector const &);
	void init(IntVector const &);
private:
	double d(size_t const & i, size_t const & c) const;
	void move(size_t const & i, size_t const & c);
	void initLoop();
private:
	void buildDiag();
	size_t _k;
	IntVector _x;
	DoubleVector _weights;

	DoubleVector _kii;

	DoubleVector _sumK;
	DoubleVector _sumW;
	DoubleVector _intra;

	DoubleVector _buffer;

	IntListVector _list;
	std::vector<IntList::iterator> _position;
};

#endif /* KMEANSDATA_HPP_ */
