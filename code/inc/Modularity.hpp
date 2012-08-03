/*
 * Modularity.hpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#ifndef MODULARITY_HPP_
#define MODULARITY_HPP_

#include "common.h"
#include "ICriterion.hpp"
class Modularity :public ICriterion{
public:
	// calcul brut
	 double eval(Data const & data)const;
	// calcul de la composante associé au label
	 double eval(Data const & data, size_t const & label)const;

	 bool isBetter(double const & candidate, double const & ref)const;
public:

		virtual ~Modularity(){}
};

#endif /* MODULARITY_HPP_ */
