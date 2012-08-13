/*
 * AbstractCriterion.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef ABSTRACTCRITERION_HPP_
#define ABSTRACTCRITERION_HPP_

#include "../utils/common.h"
#include "../interfaces/ICriterion.hpp"
#include "../interfaces/ISolution.hpp"

/*
 * à templatiser sur ISolution
 */
template<ICriterion::Sense sense, ICriterion::Kind kind>
class AbstractCriterion: public ICriterion {
public:
	virtual ~AbstractCriterion();
public:
	bool isBetter(Double candidate, Double ref) const;
	bool isPartitioning() const;

	bool canShift(ISolution const & solution, size_t const & node,
			size_t const & newLabel) const;
};

template<ICriterion::Sense sense, ICriterion::Kind kind> inline
bool AbstractCriterion<sense, kind>::isBetter(Double candidate,
		Double ref) const {
	return sense == Maximization ?
			candidate > ref + 1e-10 : candidate < ref - 1e-10;
}

template<ICriterion::Sense sense, ICriterion::Kind kind> inline
bool AbstractCriterion<sense, kind>::isPartitioning() const {
	return kind == Partitioning;
}
template<ICriterion::Sense sense, ICriterion::Kind kind>
inline AbstractCriterion<sense, kind>::~AbstractCriterion() {

}

template<ICriterion::Sense sense, ICriterion::Kind kind>
inline bool AbstractCriterion<sense, kind>::canShift(ISolution const & solution,
		size_t const & node, size_t const & newLabel) const {
	size_t const oldLabel(solution.partition().label(node));
	// In any cas, a shift is consider if oldLabel != newLabel
	// When partitioning, we cannot destroy a label or create a new one
	if (kind == Partitioning)
		return oldLabel != newLabel
				&& solution.partition().sizeOfLabel(oldLabel) > 1
				&& solution.partition().sizeOfLabel(newLabel) > 0;
	else
		return oldLabel != newLabel;
}

#endif /* ABSTRACTCRITERION_HPP_ */
