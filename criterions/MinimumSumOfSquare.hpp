/*
 * Mssc.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef MINIMUM_SUM_OF_SQUARE_HPP_
#define MINIMUM_SUM_OF_SQUARE_HPP_

#include "AbstractCriterion.hpp"
#include "Cut.hpp"
class MinimumSumOfSquare: public AbstractCriterion<ICriterion::Minimization, ICriterion::Partitioning>,
		Cut {
public:
	// calcul brut
	double eval(IGraphPartition const & data) const;
	// calcul de la composante associé au label
	double eval(IGraphPartition const & data, size_t const & label) const;
public:
	virtual ~MinimumSumOfSquare();
};

#endif /* MINIMUM_SUM_OF_SQUARE_HPP_ */
