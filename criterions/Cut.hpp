/*
 * Cut.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef CUT_HPP_
#define CUT_HPP_

#include "../utils/common.h"

class Cut {
public:
public:
	// calcul brut
	void getCut(IGraphPartition const & data, DoubleVector &) const;
	// calcul de la composante associé au label
	Double getCut(IGraphPartition const & data, size_t const & label) const;
	//
	Double2 getDeltaShift(IGraphPartition const & data, size_t const & node,
			size_t const & newLabel) const;
	 Double2 getDeltaShift(IGraphPartition const & data,
			size_t const & node, size_t const & newLabel,
			DoubleVector const & intra) const;

};

#endif /* CUT_HPP_ */
