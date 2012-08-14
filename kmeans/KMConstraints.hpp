/*
 * KMConstraints.hpp
 *
 *  Created on: 14 août 2012
 *      Author: manuel
 */

#ifndef KMCONSTRAINTS_HPP_
#define KMCONSTRAINTS_HPP_

#include "../utils/common.h"

class KMConstraints {
public:
	typedef std::pair<size_t, size_t> Constraint;
	typedef std::set<Constraint> Constraints;
public:
	KMConstraints(size_t n = 0);
	virtual ~KMConstraints();

	void newCtr(size_t i, size_t j);

	void clear();

	IntSet const & get(size_t obs) const;
private:
	Constraints _all;
	std::vector<IntSet> _byObs;
};

#endif /* KMCONSTRAINTS_HPP_ */
