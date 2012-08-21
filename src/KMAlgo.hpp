/*
 * KMeansAlgo.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef KMEANSALGO_HPP_
#define KMEANSALGO_HPP_

#include "../src/common.h"

#include "../src/KMPartition.hpp"
#include "../src/RectMatrix.hpp"
#include "../src/Partition.hpp"
#include "../src/Timer.hpp"

class KMAlgo {
public:
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
	typedef std::multimap<Double, size_t, std::greater<Double> > Distances;
public:
	static Double ComputeMssc(IPartition const &, KMInstance const &);
public:
	KMAlgo(KMPartition &);
	virtual ~KMAlgo();

	void hMeansLoop(Moves &);
	void hMeans(size_t maxIte);

	void singleton();

	void kMeans(size_t maxIte);

	KMPartition & partition();
public:
	template<class T> void out(std::string const &, T const&) const;

	void out(std::ostream &) const;
	void headers(std::ostream &);

	Double computeCost() const;
Double cost()const;
	void computeDistances();
	void shift(size_t node, size_t to);

	void apply(Move const &);
	void apply(Moves const &);

	bool checkCost() const;
	bool checkWeights() const;

	std::pair<size_t, Double> getClosest(size_t i) const;
	std::pair<size_t, Double> getBest(size_t i) const;
	bool feasible(size_t i, size_t j) const;
	Double getDelta(size_t i, size_t l, size_t j) const;
	Double getDelta(size_t i, size_t j) const;
	void checkDelta(size_t i, size_t j);
	void checkCenters() const;
private:
	KMPartition & _input;

	DoubleVector _d;
	Double _cost;
	Double _old;
	size_t _ite;

	IndexedList _pertLabels;
	IndexedList _pertNodes;

	Timer _timer;
	std::string _name;
	Distances _distances;

	IntVector _buffer;

	static std::string const HMEANS;
	static std::string const KMEANS;

};

template<class T> inline
void KMAlgo::out(std::string const & name, T const& value) const {
	OUT<< std::setw(25) << name;
	OUT << std::setw(15) << value;
	OUT << "\n";
}

#endif /* KMEANSALGO_HPP_ */

