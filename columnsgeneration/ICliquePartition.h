/*
 * ICliquePartion.h
 *
 *  Created on: 14 mars 2014
 *      Author: manuel
 */

#ifndef I_CLIQUE_PARTITION_H_
#define I_CLIQUE_PARTITION_H_

#include "common.h"
class CliquePartitionProblem;

class ICliquePartition {
 public:
  ICliquePartition(CliquePartitionProblem const &);
  virtual ~ICliquePartition();
 public:
  virtual double score() const=0;

  virtual double & score()=0;

  virtual double computeCost() const=0;

  virtual bool shift(int obs, int to)=0;

  virtual int label(int obs)=0;

  virtual void checkScore() const = 0;

  virtual CliquePartitionProblem const & problem() const;

  virtual IndexedList const & usedLabels() const = 0;
 private:
  CliquePartitionProblem const & _problem;
};

inline ICliquePartition::ICliquePartition(
    CliquePartitionProblem const & problem)
    : _problem(problem) {
}

inline ICliquePartition::~ICliquePartition() {
}

#endif /* ICLIQUEPARTION_H_ */
