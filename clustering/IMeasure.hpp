/*
 * IMeasure.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef IMEASURE_HPP_
#define IMEASURE_HPP_

#include "common.h"

class IPartition;

/**
 * Abstraction which let compute a metric to compare two partitions of a same graph
 */
class IMeasure {
public:
	/**
	 * Compute the metric
	 */
	virtual Double compute(IPartition const & actual,
			IPartition const & candidate) = 0;
	virtual ~IMeasure();
};

inline IMeasure::~IMeasure() {
}

#endif /* IMEASURE_HPP_ */
