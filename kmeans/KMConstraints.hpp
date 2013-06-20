/*
 * KMConstraints.hpp
 *
 *  Created on: 14 août 2012
 *      Author: manuel
 */

#ifndef KMCONSTRAINTS_HPP_
#define KMCONSTRAINTS_HPP_

#include "common.h"

/**
 * Represents pairs of observations of a graph.
 * Those pairs may be seen as constraints: it may represent pairs of observations which
 * must (resp. mustn't) be in the same cluster
 */
class KMConstraints {
public:
	typedef std::pair<size_t, size_t> Constraint;
	typedef std::set<Constraint> Constraints;
	typedef Constraints::const_iterator const_iterator;
public:
  /**
   * @param nbObs the number of observations of the graph refered by this class
   */
	KMConstraints(size_t nbObs = 0);
	virtual ~KMConstraints();

  /**
   * Add the pair (i, j)
   */
	void newCtr(size_t i, size_t j);

  /**
   * Forget every pair of observations
   */
	void clear();

  /**
   * @return Every observation linked to the given one
   */
	IntSet const & get(size_t obs) const;

  /**
   * @return The total number of pairs
   */
	size_t size() const;
	const_iterator begin() const;
	const_iterator end() const;
	Constraints const & all()const;
private:
  /**
   * List of every pairs
   */
	Constraints _all;

  /**
   * Contains the same data than #_all, but grouped by observations
   */
	std::vector<IntSet> _byObs;
};
inline KMConstraints::Constraints const & KMConstraints::all()const{
	return _all;
}
inline KMConstraints::KMConstraints(size_t nbObs) :
		_all(), _byObs(nbObs) {

}

inline KMConstraints::~KMConstraints() {
}

inline void KMConstraints::newCtr(size_t i, size_t j) {
  assert(i < _byObs.size() && j < _byObs.size());

	if (i != j) {
		_all.insert(std::make_pair(std::min(i, j), std::max(i, j)));
		_byObs[i].insert(j);
		_byObs[j].insert(i);
	}
}

inline IntSet const & KMConstraints::get(size_t obs) const {
	return _byObs[obs];
}
inline void KMConstraints::clear() {
	_all.clear();
	for (auto & s : _byObs)
		s.clear();
}

inline size_t KMConstraints::size() const {
	return _all.size();
}

inline KMConstraints::const_iterator KMConstraints::begin() const {
	return _all.begin();
}
inline KMConstraints::const_iterator KMConstraints::end() const {
	return _all.end();

}

#endif /* KMCONSTRAINTS_HPP_ */
