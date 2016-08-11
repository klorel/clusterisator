/*
 * LabelPropagation.h
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef LABELPROPAGATION_HPP_
#define LABELPROPAGATION_HPP_

#include "../columnsgeneration/CliquePartitionProblem.h"
#include "common.h"

#include "BipartiteGraph.h"
#include "ModularityBPartition.h"

#include "ICliquePartition.h"

class LabelPropagation2 {
 public:
  LabelPropagation2(ICliquePartition &solution);
  virtual ~LabelPropagation2();

  bool operator()(int);
  bool operator()();
 private:
  void buildGraph();
 private:
  CliquePartitionProblem const & _input;
  ICliquePartition & _solution;
  AdjencyGraph _graph;
};
#endif /* LABELPROPAGATION_HPP_ */
