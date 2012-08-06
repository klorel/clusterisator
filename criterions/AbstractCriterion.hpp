/*
 * AbstractCriterion.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef ABSTRACTCRITERION_HPP_
#define ABSTRACTCRITERION_HPP_

#include "common.h"
#include "ICriterion.hpp"

template<ICriterion::Sense sense, ICriterion::Kind kind>
class AbstractCriterion: public ICriterion {
public:
	virtual ~AbstractCriterion();
public:
	bool isBetter(double const & candidate, double const & ref) const;
	bool isPartitioning() const;
};

template<ICriterion::Sense sense, ICriterion::Kind kind> inline
bool AbstractCriterion<sense, kind>::isBetter(double const & candidate,
		double const & ref) const {
	return sense == Minimization ?
			candidate > ref + 1e-10 : candidate < ref - 1e-10;
}

template<ICriterion::Sense sense, ICriterion::Kind kind> inline
bool AbstractCriterion<sense, kind>::isPartitioning() const {
	return kind == Partitioning;
}
template<ICriterion::Sense sense, ICriterion::Kind kind>
inline AbstractCriterion<sense, kind>::~AbstractCriterion() {

}
#endif /* ABSTRACTCRITERION_HPP_ */
