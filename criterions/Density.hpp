/*
 * Modularity.hpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#ifndef DENSITY_HPP_
#define DENSITY_HPP_

#include "AbstractCriterion.hpp"

class Density: public AbstractCriterion<ICriterion::Maximization, ICriterion::Free>
{
public:
	// calcul brut
	double eval(IGraphPartition const & data) const;
	// calcul de la composante associé au label
	double eval(IGraphPartition const & data, size_t const & label) const;
public:
	virtual ~Density();
};

#endif /* MODULARITY_HPP_ */
