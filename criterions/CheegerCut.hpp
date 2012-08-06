/*
 * CheegerCut.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef CHEEGERCUT_HPP_
#define CHEEGERCUT_HPP_

#include "AbstractCriterion.hpp"
#include "Cut.hpp"
class CheegerCut: public AbstractCriterion<ICriterion::Minimization,
		ICriterion::Partitioning>, Cut {
public:
	// calcul brut
	double eval(IExtendedPartition const & data) const;
	// calcul de la composante associé au label
	double eval(IExtendedPartition const & data, size_t const & label) const;

public:

	virtual ~CheegerCut();
};

#endif /* CHEEGERCUT_HPP_ */
