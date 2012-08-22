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
	void out() const;
	void headers();

	Double computeCost() const;
	Double cost() const;
	void computeDistances();

	bool shift(size_t node, size_t to);
	bool apply(Move const &);
	bool apply(Moves const &);

	bool checkCost() const;
	bool checkWeights() const;

	std::pair<size_t, Double> getClosest(size_t i) const;
	std::pair<size_t, Double> getBest(size_t i) const;

	bool feasible(size_t i, size_t j) const;

	Double getDelta(size_t i, size_t l, size_t j) const;
	Double getDelta(size_t i, size_t j) const;
	void checkDelta(size_t i, size_t j);
	void checkCenters() const;
	bool & isTraceOn();
	bool isTraceOn() const;
private:
	KMPartition & _input;

	DoubleVector _d;
	Double _cost;
	Double _old;
	size_t _ite;

	//	IndexedList _pertLabels;
	//	IndexedList _pertNodes;

	Timer _timer;
	std::string _name;
	Distances _distances;

	IntVector _buffer;
	bool _isTraceOn;
	static std::string const HMEANS;
	static std::string const KMEANS;

};

class MsscData {
public:
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
	typedef std::multimap<Double, size_t, std::greater<Double> > Distances;
public:
	void out() const;
	void headers();

	Double computeCost() const;
	Double cost() const;
	void computeDistances();

	void shift(size_t node, size_t to);

	void apply(Move const &);
	void apply(Moves const &);

	std::pair<size_t, Double> getClosest(size_t i) const;
	std::pair<size_t, Double> getBest(size_t i) const;

	bool feasible(size_t i, size_t j) const;

	Double getDelta(size_t i, size_t l, size_t j) const;
	Double getDelta(size_t i, size_t j) const;

	bool checkCost() const;
	bool checkWeights() const;
	void checkDelta(size_t i, size_t j);
	void checkCenters() const;

	bool & isTraceOn();
	bool isTraceOn() const;

public:
	KMPartition _input;
	// les distances au centroids
	DoubleVector _d;
	// le cout
	Double _cost;
	// l'ancien cout
	Double _old;
	// les distances triées des noeuds au centroid
	Distances _distances;

};

#endif /* KMEANSALGO_HPP_ */

