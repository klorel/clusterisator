/*
 * KMConstraints.hpp
 *
 *  Created on: 14 août 2012
 *      Author: manuel
 */

#ifndef KMCONSTRAINTS_HPP_
#define KMCONSTRAINTS_HPP_

#include "src/common.h"

class KMConstraints {
public:
	typedef std::pair<size_t, size_t> Constraint;
	typedef std::set<Constraint> Constraints;
	typedef Constraints::const_iterator const_iterator;
public:
	KMConstraints(size_t n = 0);
	KMConstraints(size_t n, IntVector const &);
	virtual ~KMConstraints();

	void newCtr(size_t i, size_t j);

	void clear();

	IntSet const & get(size_t obs) const;

	size_t size() const;
	const_iterator begin() const;
	const_iterator end() const;

private:
	Constraints _all;
	std::vector<IntSet> _byObs;
};

inline KMConstraints::KMConstraints(size_t n) :
		_all(), _byObs(n) {

}
inline KMConstraints::KMConstraints(size_t n, IntVector const & values) :
		_all(), _byObs(n) {
	size_t i(0);
	while (i < values.size() && i + 1 < values.size()) {
		newCtr(values[i], values[i + 1]);
		i += 2;
	}
}

inline KMConstraints::~KMConstraints() {
}

inline void KMConstraints::newCtr(size_t i, size_t j) {
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
