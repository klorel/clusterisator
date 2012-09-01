/*
 * IMesure.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef IMESURE_HPP_
#define IMESURE_HPP_

#include "src/common.h"

class IPartition;

/**
 * Abstraction which let compute a metric to compare two partitions of a same graph
 */
class IMesure {
public:
  /**
   * Compute the metric
   */
  virtual Double compute(IPartition const & actual, IPartition const & candidate) = 0;
  virtual ~IMesure();
};

inline IMesure::~IMesure() {
}

#endif /* IMESURE_HPP_ */
