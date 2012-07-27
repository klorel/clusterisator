/*
 * Modularity.hpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#ifndef DENSITY_HPP_
#define DENSITY_HPP_

#include "common.h"
#include "ICriterion.hpp"

class Density: public ICriterion {
public:
	// calcul brut
	double eval(Data const & data) const;
	// calcul de la composante associé au label
	double eval(Data const & data, size_t const & label) const;

	bool isBetter(double const & candidate, double const & ref) const;
public:
	virtual ~Density(){}
};

#endif /* MODULARITY_HPP_ */
