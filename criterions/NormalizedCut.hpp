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

class NormalizedCut: public AbstractCriterion<ICriterion::Maximization,
		ICriterion::Partitioning>, Cut {
public:
	// calcul brut
	Double eval(IGraphPartition const & data) const;
	// calcul de la composante associé au label
	Double eval(IGraphPartition const & data, size_t const & label) const;
public:
	virtual ~NormalizedCut();
public:
	Double2 getDelta2Shift(IGraphPartition const & data, size_t const & node,
			size_t const & newLabel, DoubleVector const & intra) const;
	Double getDeltaShift(IGraphPartition const & data, size_t const & node,
			size_t const & newLabel, DoubleVector const & intra) const;

};

#endif /* NORMALIZEDCUT_HPP_ */
