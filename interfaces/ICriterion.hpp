/*
 * ICriterion.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef ICRITERION_HPP_
#define ICRITERION_HPP_

#include "common.h"
class ICriterion {
public:
	enum Sense {
		Minimization, Maximization
	};

	enum Kind {
		Partitioning, Free
	};
public:
	// calcul brut
	virtual double eval(IExtendedPartition const & data) const = 0;
	// calcul de la composante associé au label
	virtual double eval(IExtendedPartition const & data,
			size_t const & label) const = 0;

	virtual bool isBetter(double const & candidate,
			double const & ref) const = 0;

	virtual bool isPartitioning() const = 0;

	virtual ~ICriterion();
public:
	virtual bool canShift(IExtendedPartition const & data, size_t const & node,
			size_t const & newLabel) const=0;
	virtual Double2 getDelta2Shift(IExtendedPartition const & data,
			size_t const & node, size_t const & newLabel,
			DoubleVector const & intra) const = 0;
	virtual double getDeltaShift(IExtendedPartition const & data,
			size_t const & node, size_t const & newLabel,
			DoubleVector const & intra) const = 0;

};

inline ICriterion::~ICriterion() {
}

#endif /* ICRITERION_HPP_ */
