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
#include "../utils/Timer.hpp"
#include "../kmeans/KMConstraints.hpp"

class KMAlgo {
public:
	typedef std::pair<size_t, Double> CentroidData;
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;

public:
	KMAlgo(RectMatrix const &);
	virtual ~KMAlgo();

	size_t getK() const;
	void set(size_t k);
	void set(Partition const &);

	void random();

	CentroidData getNearest(size_t i) const;
	Double getDistance(size_t i, size_t k) const;
	Double getDistance(size_t i) const;
	void loop(Moves &);

	void run(size_t maxIte);

	void newMustLink(size_t, size_t);
	void newCannotLink(size_t, size_t);

	template<class T>
	void out(std::ostream &, std::string const &, T const&) const;

	void out(std::ostream &) const;
	void headers(std::ostream &) const;
private:
	Double size(size_t k) const;
	void computeCenters(RectMatrix &) const;
	void computeDistances();
	void apply(Move const &);
	void apply(Moves const &);
	void move(size_t node, size_t to);

	void check(std::string const & = "") const;
	void check(size_t) const;
	bool checkCost() const;

	void getObs(IntSet &);

	Double computeCost() const;
private:
	RectMatrix const & _input;
	// current centers
	RectMatrix _centers;
	//
	Partition _partition;
	DoubleVector _d;
	Double _cost;
	Double _old;
	size_t _ite;
	size_t _nbLabels;
	IntSet _pertObs;
	IntSet _pertLabels;
	Timer _timer;

	KMConstraints _mustLink;
	KMConstraints _cannotLink;

	std::multimap<Double, size_t, std::greater<Double> > _distances;
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
	for (size_t d(0); d < _input.getM(); ++d) {
		result += std::pow(_input.get(i, d) - _centers.get(k, d) / size(k), 2);
	}
//	return std::sqrt(result);
	return result;
}

#endif /* KMEANSALGO_HPP_ */

