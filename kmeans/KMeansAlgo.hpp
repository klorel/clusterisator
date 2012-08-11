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

#include <tuple>
class KMeansAlgo {
public:
	typedef std::pair<size_t, Double> CentroidData;
	typedef std::pair<size_t, CentroidData> Move;
	typedef std::vector<Move> Moves;
public:
	KMeansAlgo(RectMatrix const &);
	virtual ~KMeansAlgo();

	size_t getK() const;
	void set(size_t k);
	void set(Partition const &);

	void random();

	std::pair<size_t, Double> getNearest(size_t i) const;
	Double getDistance(size_t i, size_t k) const;
	void loop(Moves &);

	void run(size_t maxIte);
	void out(std::ostream &, size_t ite, Double old) const;
private:
	Double size(size_t k) const;
	void computeCenters();
	void computeCost();
	void apply(Move const &);
private:
	RectMatrix const & _input;
	// current centers
	RectMatrix _centers;
	//
	Partition _partition;
	DoubleVector _d;
	Double _cost;
};

#endif /* KMEANSALGO_HPP_ */
