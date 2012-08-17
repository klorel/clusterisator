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
	KMAlgo(KMPartition &);
	virtual ~KMAlgo();

	void random();

	void loop(Moves &);
	void singleton();

	void run(size_t maxIte);
	void run2();
	template<class T>
	void out(std::ostream &, std::string const &, T const&) const;

	void out(std::ostream &) const;
	void headers(std::ostream &) const;
	Double computeCost() const;

	void shift(size_t node, size_t to);

	void computeDistances();
	void apply(Move const &);
	void apply(Moves const &);

	bool checkCost() const;
	bool checkWeights() const;

	std::pair<size_t, Double> getClosest(size_t i) const;
	std::pair<size_t, Double> getBest(size_t i) const;
	Double getDelta(size_t i, size_t l, size_t j) const;
private:
	KMPartition & _input;

	DoubleVector _d;
	Double _cost;
	Double _old;
	size_t _ite;

	IndexedList _pertObs;
	IndexedList _pertLabels;
	Timer _timer;

	Distances _distances;

	IntList _empty;
};

template<class T> inline
void KMAlgo::out(std::ostream & stream, std::string const & name,
		T const& value) const {
	stream << std::setw(25) << name;
	stream << std::setw(15) << value;
	stream << "\n";
}

#endif /* KMEANSALGO_HPP_ */

