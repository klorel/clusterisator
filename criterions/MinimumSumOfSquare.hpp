/*
 * Mssc.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef MINIMUM_SUM_OF_SQUARE_HPP_
#define MINIMUM_SUM_OF_SQUARE_HPP_

#include "AbstractCriterion.hpp"

class MinimumSumOfSquare: public AbstractCriterion<ICriterion::Minimization,
		ICriterion::Partitioning> {
public:
	// calcul brut
	Double eval(ISolution const & data) const;
	// calcul de la composante associé au label
	Double eval(ISolution const & data, size_t const & label) const;
public:
	virtual ~MinimumSumOfSquare();
};

#endif /* MINIMUM_SUM_OF_SQUARE_HPP_ */
