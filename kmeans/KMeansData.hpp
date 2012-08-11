/*
 * KMeansData.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef KMEANSDATA_HPP_
#define KMEANSDATA_HPP_

#include "common.h"
#include "IRawData.hpp"

struct ClusterData {
	//
	Double weight;
	//
	Double intra;
	//
	IntList list;

	ClusterData() :
			weight(0), intra(0), list() {
	}
};

typedef std::vector<ClusterData> ClusterDatas;

class KMeansData {
public:
	KMeansData(IRawData const &, size_t const & k);
	virtual ~KMeansData();
public:
	bool loop(size_t const & maxIte = 0);
	bool loop(IndexedList const & sub, size_t const & maxIte = 0);
	size_t getClosest(size_t const & i);
	size_t getK() const;
	void init(IntVector const &);
private:
	Double getD(size_t const & i, size_t const & c) const;
	void move(size_t const & i, size_t const & c);
	void initLoop();
	void getIntra();
	Double getLinearPart(size_t const & i, size_t const & c) const;
	Double getIntra(size_t const & c) const;
private:
	void buildDiag();

	IRawData const & _data;

	IntVector _x;

	DoubleVector _kii;
	ClusterDatas _clusters;
	std::vector<IntList::iterator> _position;
};

#endif /* KMEANSDATA_HPP_ */
