/*
 * RandIndex.h
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#ifndef RANDINDEX_HPP_
#define RANDINDEX_HPP_

#include "common.h"
#include "IMeasure.h"
#include "IPartition.h"

class RandIndex : public IMeasure {
 public:
  RandIndex();
  virtual ~RandIndex();
 public:

  Double compute(IPartition const & real, IPartition const & candidate);
 private:

};

inline RandIndex::RandIndex() {
}

inline RandIndex::~RandIndex() {
}

inline Double RandIndex::compute(IPartition const & real,
                                 IPartition const & candidate) {
  // true positive : same real - same proposed
  Double TP(0);
  // true negative : different real - different proposed
  Double TN(0);
  // false positive : same real - different proposed
  Double FP(0);
  // false negative : different real - same proposed
  Double FN(0);
  for (int i(0); i < real.nbObs(); ++i) {
    for (int j(i + 1); j < real.nbObs(); ++j) {
      if (real.label(i) == real.label(j)) {
        if (candidate.label(i) == candidate.label(j))
          ++TP;
        else
          ++FP;
      } else {
        if (candidate.label(i) != candidate.label(j))
          ++TN;
        else
          ++FN;
      }
    }
  }
  return (TP + TN) / (TP + TN + FP + FN);
}

#endif /* RANDINDEX_HPP_ */
