#ifndef VNS_GENERATOR_SOLUTION_HPP
#define VNS_GENERATOR_SOLUTION_HPP

#include "../clustering/IndexedList.h"
#include "Column.h"
#include "Decision.h"
#include "gencol.h"

class ClusteringProblem;

class Node;
class VnsGeneratorSolution {
 public:
  VnsGeneratorSolution(ClusteringProblem const *, DoubleVector const * dual);
  virtual ~VnsGeneratorSolution();
  VnsGeneratorSolution & operator=(VnsGeneratorSolution const &);
 public:
  void swap(int id);
  void swap(int id, Double, Double);

  Double computeCost() const;
  Double computeReducedCost() const;
  void build(Column &);
  int violation(DecisionList const &) const;
  void clear();
  bool check() const;
 public:
  static bool IsBetter(VnsGeneratorSolution const & p,
                       VnsGeneratorSolution const & q,
                       DecisionList const & decisions);
  Double dual(int) const;
  void setDual(DoubleVector const &);
 private:
  ClusteringProblem const * _input;
  DoubleVector const * _dual;
 public:
  IndexedList _v;

  Double _cost;
  Double _reducedCost;
};

inline Double VnsGeneratorSolution::dual(int n) const {
  return (*_dual)[n];
}
#endif /* GRAPH_HPP */
