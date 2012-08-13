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

class KMAlgo {
public:
	typedef std::pair<size_t, Double> CentroidData;
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
	typedef std::pair<size_t, size_t> Constraint;
	typedef std::vector<Constraint> Constraints;
public:
	KMAlgo(RectMatrix const &);
	virtual ~KMAlgo();

	size_t getK() const;
	void set(size_t k);
	void set(Partition const &);

	void random();

	CentroidData getNearest(size_t i) const;
	Double getDistance(size_t i, size_t k) const;
	void loop(Moves &);

	void run(size_t maxIte);

	void newMustLink(size_t, size_t);
	void newCannotLink(size_t, size_t);

	template<class T>
	void out(std::ostream &, std::string const &, T const&) const;

	void out(std::ostream &, size_t ite, Double old) const;
private:
	Double size(size_t k) const;
	void computeCenters(RectMatrix &) const;
	void computeCost();
	void apply(Move const &);
	void apply(Moves const &);
	void check(std::string const & = "") const;
	void check(size_t) const;

private:
	RectMatrix const & _input;
	// current centers
	RectMatrix _centers;
	//
	Partition _partition;
	DoubleVector _d;
	Double _cost;

	Constraints _mustLink;
	Constraints _cannotLink;

	// à faire : créer les graphes des contraintes associés à chaque type pour pouvoir tester par noeud
};

template<class T> inline
void KMAlgo::out(std::ostream & stream, std::string const & name,
		T const& value) const {
	stream << std::setw(25) << name;
	stream << std::setw(15) << value;
	stream << "\n";
}

#endif /* KMEANSALGO_HPP_ */

