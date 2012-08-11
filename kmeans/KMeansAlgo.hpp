/*
 * KMeansAlgo.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef KMEANSALGO_HPP_
#define KMEANSALGO_HPP_

#include "../utils/common.h"

#include "../data/RectMatrix.hpp"
#include "../data/Partition.hpp"

class KMeansAlgo {
public:
	KMeansAlgo(RectMatrix const &);
	virtual ~KMeansAlgo();

	size_t getK() const;
	void set(size_t k);
	void set(Partition const &);

	void random();

	size_t getNearest(size_t i) const;
	Double getDistance(size_t i, size_t k) const;
	void loop(std::vector<std::pair<size_t, size_t> > & );

	void run(size_t maxIte);
private:
	void computeCenters();
private:
	RectMatrix const & _input;
	// current centers
	RectMatrix _centers;
	//
	Partition _partition;
};

#endif /* KMEANSALGO_HPP_ */
