/*
 * KMeansData.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef KMEANSDATA_HPP_
#define KMEANSDATA_HPP_

#include "RawData.hpp"

struct ClusterData {
	//
	double weight;
	//
	double intra;
	//
	IntList list;

	ClusterData() :
			weight(0), intra(0), list() {
	}
};

typedef std::vector<ClusterData> ClusterDatas;

class KMeansData: public RawData {
public:
	KMeansData(size_t const & n, size_t const & m, size_t const & k);
	virtual ~KMeansData();
public:
	bool loop(size_t const & maxIte = 0);
	bool loop(IndexedList const & sub,size_t const & maxIte = 0);
	size_t getClosest(size_t const & i);
	size_t getK() const;
	void setWeights(DoubleVector const &);
	void init(IntVector const &);
private:
	double getD(size_t const & i, size_t const & c) const;
	void move(size_t const & i, size_t const & c);
	void initLoop();
	void getIntra();
	double getLinearPart(size_t const & i, size_t const & c) const;
	double getIntra(size_t const & c) const;
private:
	void buildDiag();

	IntVector _x;
	DoubleVector _weights;

	DoubleVector _kii;
	ClusterDatas _clusters;
	std::vector<IntList::iterator> _position;
};

#endif /* KMEANSDATA_HPP_ */
