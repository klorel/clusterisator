/*
 * Modularity.hpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#ifndef MODULARITY_HPP_
#define MODULARITY_HPP_

#include "AbstractCriterion.hpp"

class Modularity: public AbstractCriterion<ICriterion::Maximization,
		ICriterion::Free> {
public:
	// calcul brut
	Double eval(IGraphPartition const & data) const;
	// calcul de la composante associé au label
	Double eval(IGraphPartition const & data, size_t const & label) const;
public:

	virtual ~Modularity();
};

#endif /* MODULARITY_HPP_ */
