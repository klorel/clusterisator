/*
 * NormalizedCut.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef NORMALIZEDCUT_HPP_
#define NORMALIZEDCUT_HPP_

#include "Cut.hpp"
#include "AbstractCriterion.hpp"

class NormalizedCut: public AbstractCriterion<ICriterion::Maximization, ICriterion::Partitioning>, Cut {
public:
	// calcul brut
	double eval(IExtendedPartition const & data) const;
	// calcul de la composante associé au label
	double eval(IExtendedPartition const & data, size_t const & label) const;
public:
	virtual ~NormalizedCut();
public:
	Double2 getDelta2Shift(IExtendedPartition const & data, size_t const & node,
			size_t const & newLabel, DoubleVector const & intra) const;
	double getDeltaShift(IExtendedPartition const & data,
				size_t const & node, size_t const & newLabel,
				DoubleVector const & intra) const;

};

#endif /* NORMALIZEDCUT_HPP_ */
