/*
 * KMeansAlgo.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef KMEANSALGO_HPP_
#define KMEANSALGO_HPP_

#include "../src/common.h"

#include "../src/KMInstance.hpp"
#include "../src/RectMatrix.hpp"
#include "../src/Partition.hpp"
#include "../src/Timer.hpp"

class KMAlgo {
public:
	typedef std::pair<size_t, Double> CentroidData;
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
	typedef std::multimap<Double, size_t, std::greater<Double> > Distances;

public:

	KMAlgo(KMInstance const &, size_t k);
	virtual ~KMAlgo();

	size_t getK() const;

	void set(Partition const &);

	void random();

	CentroidData getClosest(size_t i) const;

	CentroidData getBest(size_t i) const;

	Double getDelta(size_t i, size_t l, size_t j) const;
	void test();
	template<bool isInsertion>
	Double getCoeff(size_t k) const;

	Double getDistance(size_t i, size_t k) const;
	Double getDistance(size_t i) const;
	void loop(Moves &);
	void singleton();

	void run(size_t maxIte);
	void run2();
	template<class T>
	void out(std::ostream &, std::string const &, T const&) const;

	void out(std::ostream &) const;
	void headers(std::ostream &) const;
private:
	Double size(size_t k) const;
	void computeCenters();
	void computeCenters(RectMatrix &) const;
	void computeDistances();
	void apply(Move const &);
	void apply(Moves const &);
	void move(size_t node, size_t to);

	bool checkCost() const;

	void getObs(IntSet &);

	Double computeCost() const;
	Double computeCost(size_t k) const;
private:
	KMInstance const & _input;
	// current centers
	RectMatrix _centers;
	//
	Partition _partition;
	DoubleVector _d;
	Double _cost;
	Double _old;
	size_t _ite;

	IndexedList _pertObs;
	IndexedList _pertLabels;
	Timer _timer;

	Distances _distances;

	IntList _empty;

	// à faire : créer les graphes des contraintes associés à chaque type pour pouvoir tester par noeud
};

template<class T> inline
void KMAlgo::out(std::ostream & stream, std::string const & name,
		T const& value) const {
	stream << std::setw(25) << name;
	stream << std::setw(15) << value;
	stream << "\n";
}

inline size_t KMAlgo::getK() const {
	return _centers.getN();
}
inline Double KMAlgo::size(size_t k) const {
	return static_cast<Double>(_partition.sizeOfLabel(k));
}

inline Double KMAlgo::getDistance(size_t i) const {
	return getDistance(i, _partition.label(i));
}
inline Double KMAlgo::getDistance(size_t i, size_t k) const {
	Double result(0);
	for (size_t d(0); d < _input.nbAtt(); ++d) {
		result += std::pow(_input.get(i, d) - _centers.get(k, d) / size(k), 2);
	}
//	return std::sqrt(result);
	return result;
}

template<> inline Double KMAlgo::getCoeff<true>(size_t k) const {
	return size(k) / (size(k) + 1);
}
template<> inline Double KMAlgo::getCoeff<false>(size_t k) const {
	return size(k) / (size(k) - 1);
}

#endif /* KMEANSALGO_HPP_ */

