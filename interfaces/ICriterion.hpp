/*
 * ICriterion.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef ICRITERION_HPP_
#define ICRITERION_HPP_

#include "../utils/common.h"
#include "../interfaces/ISolution.hpp"

/*
 * à templatiser sur ISolution
 */

class ICriterion {
public:
	enum Sense {
		Minimization, Maximization
	};

	enum Kind {
		Partitioning, Free
	};
public:
	// calcul brut
	virtual Double eval(ISolution const & data) const = 0;
	// calcul de la composante associé au label
	virtual Double eval(ISolution const & data, size_t const & label) const = 0;

	virtual bool isBetter(Double candidate, Double ref) const = 0;

	virtual bool isPartitioning() const = 0;

	virtual ~ICriterion();
public:
	virtual bool canShift(ISolution const & data, size_t const & node,
			size_t const & newLabel) const=0;
	virtual Double2 getDelta2Shift(ISolution const & data, size_t const & node,
			size_t const & newLabel) const = 0;
	virtual Double getDeltaShift(ISolution const & data, size_t const & node,
			size_t const & newLabel) const = 0;

};

inline ICriterion::~ICriterion() {
}

#endif /* ICRITERION_HPP_ */
