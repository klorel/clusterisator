#ifndef BRANCH_AND_BOUND_HPP
#define BRANCH_AND_BOUND_HPP

#include "gencol.h"
#include "IOracle.h"

#include "LpMaster.h"
#include "ColumnGenerator.h"

class Node;
class ClusteringProblem;
class BranchAndBound {
 public:
  BranchAndBound(ClusteringProblem const &);
  virtual ~BranchAndBound();
 public:
  ClusteringProblem const * _input;

  void columnGeneration();

  void init();
  void run();
  void treat(Node * node);
  void printTree(std::ostream & = std::cout) const;
  std::ostream & output();
  void setOutput(std::ostream & = std::cout);
  LpMaster & master();
  LpMaster const & master() const;

  void writeSolution() const;

  LpMaster * _master;

  ColumnGenerator _columnGenerator;

  double bestFeasible() const;
  double bestPossible() const;
 private:
  Node * _root;
  Node * _current;

  DecisionList _decision;

  std::ostream * _output;
  std::multimap<Double, Node *, std::greater<Double>> _nodesByUpperBounds;

  Double _bestFeasible;
  Double _bestPossible;
  FractionnarySolution _bestSolution;

};

#endif 
