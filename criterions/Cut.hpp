/*
 * Cut.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef CUT_HPP_
#define CUT_HPP_

#include "common.h"

class Cut {
public:
	// calcul brut
	void getCut(IExtendedPartition const & data, DoubleVector &) const;
	// calcul de la composante associé au label
	double getCut(IExtendedPartition const & data, size_t const & label) const;
};

#endif /* CUT_HPP_ */
