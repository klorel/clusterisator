/*
 * KMPartition.hpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#ifndef KMPARTITION_HPP_
#define KMPARTITION_HPP_

#include "../src/common.h"
#include "../src/Timer.hpp"
#include "../src/Partition.hpp"
#include "../src/KMInstance.hpp"

class KMPartition;

class KMPartition: public Partition {
public:
	typedef std::multimap<Double, size_t, std::greater<Double> > Distances;
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
public:
	KMPartition(KMInstance const &, size_t k = 1);
	KMPartition(KMInstance const &, Partition const &);
	virtual ~KMPartition();

	void computeCenters();
	void computeCenters(RectMatrix &) const;

	RectMatrix const & centers() const;
	KMInstance const & instance() const;
	Double cst() const;

	bool shift(size_t obs, size_t to);

	bool shift(Move const &);
	bool shift(Moves const &);

	size_t getK() const;
	template<bool isInsertion> Double getCoeff(size_t i, size_t k) const;

	Double getDistance(size_t i, size_t k) const;
	Double getDistance(size_t i) const;
	KMPartition & operator=(KMPartition const &);
	KMConstraints const & mustLinks() const;
	KMConstraints const & cannotLinks() const;
	IntSet const & mustLinks(size_t i) const;
	IntSet const & cannotLinks(size_t i) const;

public:
	Double computeCost() const;
	Double & cost();
	Double cost() const;
	void computeDistances();

	std::pair<size_t, Double> getClosest(size_t obs) const;
	std::pair<size_t, Double> getBest(size_t obs) const;

	bool feasible(size_t obs, size_t to) const;

	Double getDelta(size_t obs, size_t from, size_t to) const;
	Double getDelta(size_t obs, size_t to) const;

	bool checkCost() const;
//	bool checkWeights() const;
	void checkDelta(size_t obs, size_t to);
	void checkCenters() const;

	bool & isTraceOn();
	bool isTraceOn() const;
	Distances & distances();
	Distances const & distances() const;

	Double d(size_t) const;
private:
	KMInstance const & _input;
	// les centroids
	RectMatrix _centers;
	// les distances au centroids
	DoubleVector _d;
	// le cout
	Double _cost;
	// l'ancien cout
	Double _old;
	// les distances triées des noeuds au centroid
	Distances _distances;
};

inline size_t KMPartition::getK() const {
	return _centers.getN();
}

inline Double KMPartition::getDistance(size_t i) const {
	return getDistance(i, label(i));
}
inline Double KMPartition::getDistance(size_t i, size_t k) const {
	Double result(0);
	for (size_t d(0); d < _input.nbAtt(); ++d) {
		if (labelWeight(k) == Zero<Double>())
			result += std::pow(_input.get(i, d), 2);
		else
			result += std::pow(
					_input.get(i, d) - _centers.get(k, d) / labelWeight(k), 2);
	}
	return result;
}

template<> inline Double KMPartition::getCoeff<true>(size_t i, size_t k) const {
	return labelWeight(k) / (labelWeight(k) + obsWeight(i));
}
template<> inline Double KMPartition::getCoeff<false>(size_t i,
		size_t k) const {
	return labelWeight(k) / (labelWeight(k) - obsWeight(i));
}

inline Double KMPartition::cst() const {
	return _input.cst();
}

inline RectMatrix const & KMPartition::centers() const {
	return _centers;
}
inline KMInstance const & KMPartition::instance() const {
	return _input;
}
inline KMConstraints const & KMPartition::mustLinks() const {
	return _input.mustLinks();
}
inline KMConstraints const & KMPartition::cannotLinks() const {
	return _input.cannotLinks();
}
inline IntSet const & KMPartition::mustLinks(size_t i) const {
	return _input.mustLinks().get(i);
}
inline IntSet const & KMPartition::cannotLinks(size_t i) const {
	return _input.cannotLinks().get(i);
}

inline KMPartition::Distances & KMPartition::distances() {
	return _distances;
}
inline KMPartition::Distances const & KMPartition::distances() const {
	return _distances;
}

inline Double KMPartition::d(size_t obs) const {
	return _d[obs];
}
#endif /* KMPARTITION_HPP_ */
